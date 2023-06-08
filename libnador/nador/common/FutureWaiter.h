#ifndef __NADOR_FUTURE_WAITER_H__
#define __NADOR_FUTURE_WAITER_H__

namespace nador
{
    template <typename T>
    concept FutureConcept = requires(T t) {
        { t.valid() };
        { t.wait() };
    };

    struct FutureWaiter
    {
        template <FutureConcept T>
        void operator()(T& future)
        {
            if (future.valid())
            {
                future.wait();
            }
        }
    };
} // namespace nador

#endif //!__NADOR_FUTURE_WAITER_H__
