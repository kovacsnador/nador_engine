#include "nador/common/ThreadPool.h"
#include "nador/log/Log.h"
#include "nador/common/ThreadUtils.h"

nador::ThreadPool::ThreadPool(uint32_t nrThreads)
{
    if(nrThreads == 0)
    {
        throw std::invalid_argument("nrThreads need to be >0");
    }

    for (uint32_t i = 0; i < nrThreads; ++i)
    {
        _workerthreads.emplace_back([this, i] {
            
            char threadName[50];
            if(snprintf ( threadName, sizeof(threadName), "worker_thread_%d", i))
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

                ++_nrRunningTasks;

                lock.unlock();

                // run task
                task();

                lock.lock();
                --_nrRunningTasks;

                if(_taskQueue.empty() && _nrRunningTasks == 0)
                {
                    _wakeUpWaiterCv.notify_one();
                }
            }
        });
    }
}

void nador::ThreadPool::enqueue(const std::vector<ThreadPoolTask>& batchTasks)
{
    {
        std::unique_lock<std::mutex> lock(_mtx);
        for (const auto& it : batchTasks)
        {
            _taskQueue.emplace(it);
        }
    }

    // notify thread
    _wakeUpWorkersCv.notify_one();
}

void nador::ThreadPool::wait()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _wakeUpWaiterCv.wait(lock, [this]{ return _taskQueue.empty() && _nrRunningTasks == 0; });
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
    return GetPriority() > rhs.GetPriority();;
}

nador::ETaskPriority nador::ThreadPoolTask::GetPriority() const noexcept
{
    return _priority;
}
