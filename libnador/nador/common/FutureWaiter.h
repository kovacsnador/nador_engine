#ifndef __NADOR_FUTURE_WAITER_H__
#define __NADOR_FUTURE_WAITER_H__

#include "nador/utils/CppMacros.h"

namespace nador
{
#ifdef NADOR_CPP20_OR_LATER
    template <typename T>
    concept FutureConcept = requires(T t) {
        { t.valid() };
        { t.wait() };
    };
#endif

    struct FutureWaiter
    {
#ifdef NADOR_CPP20_OR_LATER
        template <FutureConcept T>
#else
        template <typename T>
#endif
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
