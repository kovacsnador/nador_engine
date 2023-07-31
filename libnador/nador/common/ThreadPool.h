#ifndef __NADOR_THREAD_POOL_H__
#define __NADOR_THREAD_POOL_H__

#include <queue>
#include <future>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <functional>
#include <concepts>
#include <any>
#include <unordered_map>
#include <optional>

#include "nador/utils/NonCopyable.h"
#include "nador/utils/Types.h"

namespace nador
{
    enum class ETaskPriority
    {
        VERY_HIGH = 0,
        HIGH,
        MEDIUM,
        LOW,
        VERY_LOW,
    };

    class ThreadPoolTask
    {
    public:
        template <typename PackagedTaskTy>
            requires std::invocable<PackagedTaskTy>
        ThreadPoolTask(std::shared_ptr<PackagedTaskTy>&& packagedTask, ETaskPriority priority)
        : _function([packagedTask = std::move(packagedTask)] { (*packagedTask)(); })
        , _priority(priority)
        {
        }

        ETaskPriority GetPriority() const noexcept;

        void operator()() const;

        bool operator<(const ThreadPoolTask& rhs) const noexcept;

    private:
        std::function<void()> _function;
        ETaskPriority         _priority;
    };

    template <typename FunctionTy, typename... Args>
        requires std::invocable<FunctionTy, Args...>
    NADOR_NODISCARD auto CreatePackagedTask(FunctionTy&& function, Args&&... args)
    {
        using returnType_t = std::result_of_t<std::decay_t<FunctionTy>(Args...)>;

        return std::make_shared<std::packaged_task<returnType_t()>>(std::bind(std::forward<FunctionTy>(function), std::forward<Args>(args)...));
    }

    class ThreadPool : private NonCopyable
    {
        template <typename KeyTy, typename ValueTy, typename Inc, typename Dec>
        class TasksTracker
        {
        public:
            void Add(KeyTy key) { _map[key] = inc(_map[key]); }

            void Erase(KeyTy key)
            {
                auto it = _map.find(key);
                if(it != _map.end())
                {
                    auto val = it->second;
                    auto [newVal, erase] = dec(val);

                    if(erase)
                    {
                        _map.erase(it);
                    }
                    else
                    {
                        _map.insert_or_assign(key, newVal);        
                    }
                }
            }

            std::optional<ValueTy> GetCount(KeyTy key) const
            {
                auto it = _map.find(key);
                if (it != _map.end())
                {
                    return it->second;
                }
                return {};
            }

            auto GetTotal() const { return _map.size(); }

        private:
            std::unordered_map<KeyTy, ValueTy> _map;

            Inc inc;
            Dec dec;
        };

        struct Add
        {
            uint32_t operator()(uint32_t val) { return ++val; }
        };

        struct Erase
        {
            std::tuple<uint32_t, bool> operator()(uint32_t val)
            {
                if (val > 0)
                {
                    --val;
                }

                return std::make_tuple(val, val == 0);
            }
        };

    public:
        ThreadPool(uint32_t nrThreads, std::string_view nameOfPool = "nador_worker_thread_");
        ~ThreadPool();

        template <typename FunctionTy, typename... Args>
            requires std::invocable<FunctionTy, Args...>
        std::future<std::result_of_t<std::decay_t<FunctionTy>(Args...)>> Enqueue(FunctionTy&& function, ETaskPriority priority, Args&&... args)
        {
            auto packagedTask = CreatePackagedTask(std::forward<FunctionTy>(function), std::forward<Args>(args)...);

            auto future = packagedTask->get_future();

            // add function to queue
            {
                std::unique_lock<std::mutex> lock(_mtx);
                _taskQueue.emplace(ThreadPoolTask(std::move(packagedTask), priority));
                _pendingTasks.Add(priority);
            }

            // notify threads
            _wakeUpWorkersCv.notify_all();

            return future;
        }

        void Enqueue(const std::vector<ThreadPoolTask>& batchTasks);

        void Wait();

        void Wait(ETaskPriority prio);

        size_t WorkerThreadCount() const noexcept;

    private:
        uint32_t _GetTaskCount(ETaskPriority prio) const noexcept;
        uint32_t _Empty() const noexcept;

        std::vector<std::thread>            _workerthreads;
        std::priority_queue<ThreadPoolTask> _taskQueue;

        TasksTracker<ETaskPriority, uint32_t, Add, Erase> _runningTasks;
        TasksTracker<ETaskPriority, uint32_t, Add, Erase> _pendingTasks;

        std::mutex              _mtx;
        std::condition_variable _wakeUpWorkersCv;
        std::condition_variable _wakeUpWaiterCv;
        bool                    _stop { false };
    };

    ThreadPool* GetThreadPool();
    void        SetThreadPool(std::unique_ptr<ThreadPool> threadPool);

} // namespace nador

#endif //!__NADOR_THREAD_POOL_H__