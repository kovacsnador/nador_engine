#include "nador/common/ThreadPool.h"
#include "nador/common/ThreadUtils.h"
#include "ThreadPool.h"

namespace nador
{
    static std::unique_ptr<ThreadPool> g_threadPool = std::make_unique<ThreadPool>(std::thread::hardware_concurrency());

    ThreadPool* GetThreadPool()
    {
        return g_threadPool.get();
    }

    void SetThreadPool(std::unique_ptr<ThreadPool> threadPool)
    {
        g_threadPool = std::move(threadPool);
    }

} // namespace nador

nador::ThreadPool::ThreadPool(uint32_t nrThreads)
{
    if (nrThreads == 0)
    {
        throw std::invalid_argument("nrThreads need to be >0");
    }

    for (uint32_t i = 0; i < nrThreads; ++i)
    {
        _workerthreads.emplace_back([this, i] {
            char threadName[50];
            if (snprintf(threadName, sizeof(threadName), "nador_worker_thread_%d", i))
            {
                SetThreadName(threadName);
            }

            for (;;)
            {
                std::unique_lock<std::mutex> lock(_mtx);
                _wakeUpWorkersCv.wait(lock, [this] { return _taskQueue.empty() == false || _stop; });

                if (_stop)
                {
                    // stop the thread
                    break;
                }

                auto task = _taskQueue.top();
                _taskQueue.pop();

                auto prio = task.GetPriority();

                _pendingTasks.Erase(prio);
                _runningTasks.Add(prio);

                lock.unlock();

                // run task
                task();

                lock.lock();
                _runningTasks.Erase(prio);

                if (_Empty() || _GetTaskCount(prio) == 0)
                {
                    _wakeUpWaiterCv.notify_all();
                }
            }
        });
    }
}

void nador::ThreadPool::Enqueue(const std::vector<ThreadPoolTask>& batchTasks)
{
    {
        std::unique_lock<std::mutex> lock(_mtx);
        for (const auto& it : batchTasks)
        {
            _pendingTasks.Add(it.GetPriority());
            _taskQueue.emplace(it);
        }
    }

    // notify thread
    _wakeUpWorkersCv.notify_all();
}

void nador::ThreadPool::Wait()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _wakeUpWaiterCv.wait(lock, [this] { return _Empty(); });
}

void nador::ThreadPool::Wait(ETaskPriority prio)
{
    std::unique_lock<std::mutex> lock(_mtx);
    _wakeUpWaiterCv.wait(lock, [this, prio] {
        return _GetTaskCount(prio) == 0;
    });
}

size_t nador::ThreadPool::WorkerThreadCount() const noexcept
{
    return _workerthreads.size();
}

uint32_t nador::ThreadPool::_GetTaskCount(ETaskPriority prio) const noexcept
{
    auto runningCount = _runningTasks.GetCount(prio);
    auto pendingCount = _pendingTasks.GetCount(prio);

    return runningCount.value_or(0) + pendingCount.value_or(0);
}

uint32_t nador::ThreadPool::_Empty() const noexcept
{
    return _taskQueue.empty() && _runningTasks.GetTotal() == 0;
}

nador::ThreadPool::~ThreadPool()
{
    {
        std::unique_lock lock(_mtx);
        _stop = true;
    }

    _wakeUpWorkersCv.notify_all();

    for (auto& it : _workerthreads)
    {
        if (it.joinable())
        {
            it.join();
        }
    }
}

void nador::ThreadPoolTask::operator()() const
{
    _function();
}

bool nador::ThreadPoolTask::operator<(const ThreadPoolTask& rhs) const noexcept
{
    return GetPriority() > rhs.GetPriority();
}

nador::ETaskPriority nador::ThreadPoolTask::GetPriority() const noexcept
{
    return _priority;
}
