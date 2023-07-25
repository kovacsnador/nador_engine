#ifndef __NADOR_BARRIER_H__
#define __NADOR_BARRIER_H__

#include <mutex>

#include "nador/utils/NonCopyable.h"

namespace nador
{
    template <typename CvTy, typename MtxTy>
    class Barrier : private NonCopyable
    {
    public:
        Barrier(size_t count)
        : _count(count)
        {
        }

        void Notify()
        {
            {
                std::scoped_lock lock { _mtx };
                ++_currentCount;
            }
            _cv.notify_one();
        }

        void Wait()
        {
            std::unique_lock lock { _mtx };
            _cv.wait(lock, [this] { return _currentCount == _count; });
        }

    private:
        CvTy  _cv {};
        MtxTy _mtx {};

        size_t       _currentCount { 0 };
        const size_t _count { 0 };
    };
} // namespace nador

#endif //!__NADOR_BARRIER_H__