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
    public:
        ThreadPool(uint32_t nrThreads);
        ~ThreadPool();

        template <typename FunctionTy, typename... Args>
            requires std::invocable<FunctionTy, Args...>
        std::future<std::result_of_t<std::decay_t<FunctionTy>(Args...)>> enqueue(FunctionTy&& function, ETaskPriority priority, Args&&... args)
        {
            auto packagedTask = CreatePackagedTask(std::forward<FunctionTy>(function), std::forward<Args>(args)...);

            auto future = packagedTask->get_future();

            // add function to queue
            {
                std::unique_lock<std::mutex> lock(_mtx);
                _taskQueue.emplace(ThreadPoolTask(std::move(packagedTask), priority));
            }

            // notify thread
            _wakeUpWorkersCv.notify_one();

            return future;
        }

        void enqueue(const std::vector<ThreadPoolTask>& batchTasks);

        void wait();

    private:
        std::vector<std::thread>            _workerthreads;
        std::priority_queue<ThreadPoolTask> _taskQueue;
        uint32_t                            _nrRunningTasks { 0 };

        std::mutex              _mtx;
        std::condition_variable _wakeUpWorkersCv;
        std::condition_variable _wakeUpWaiterCv;
        bool                    _stop { false };
    };

} // namespace nador

#endif //!__NADOR_THREAD_POOL_H__