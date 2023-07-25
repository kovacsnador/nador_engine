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

        explicit FinalAction(value_type&& func) noexcept
        : _func(std::forward<value_type>(func))
        {
        }

        FinalAction(FinalAction&& other) noexcept
        : _func(std::move(other._func))
        , _invoke(std::exchange(other._invoke, false))
        {
        }

        ~FinalAction()
        {
            if (_invoke && _func)
            {
                _func();
            }
        }

    private:
        value_type _func {};
        bool       _invoke { true };
    };

    template <typename T>
        requires std::invocable<T>
    auto Finally(T&& func)
    {
        return FinalAction<T>(std::forward<T>(func));
    }
} // namespace nador

#endif //!__NADOR_FINALLY_H__