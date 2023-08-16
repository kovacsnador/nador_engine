#ifndef __NADOR_FINALLY_H__
#define __NADOR_FINALLY_H__

#include <utility>

#include "nador/utils/NonCopyable.h"

namespace nador
{
    template <typename T>
        requires std::invocable<T>
    class FinalAction : private OnlyMoveable
    {
    public:
        using value_type = T;

        explicit FinalAction(value_type&& func)
        : _func{std::move(func)}
        {
        }

        FinalAction(FinalAction&& other) noexcept
        : _func{std::move(other._func)}
        , _own{std::exchange(other._own, false)}
        {
        }

        ~FinalAction()
        {
            if (_own)
            {
                _func();
            }
        }

    private:
        value_type _func {};
        bool       _own { true };
    };

    template <typename T>
        requires std::invocable<T>
    auto Finally(T&& func)
    {
        return FinalAction<T>(std::forward<T>(func));
    }
} // namespace nador

#endif //!__NADOR_FINALLY_H__