#ifndef __NADOR_FINALLY_H__
#define __NADOR_FINALLY_H__

#include <utility>
#include <type_traits>

#include "nador/utils/CppMacros.h"

namespace nador
{
    template <typename T, typename = std::enable_if_t<!std::is_reference_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T>>>
#ifdef NADOR_CPP20_OR_LATER
        requires std::invocable<T>
#endif
    class FinalAction
    {
    public:
        using value_type = T;

        explicit FinalAction(const value_type& func)
        : _func{func}
        {
        }

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
#ifdef NADOR_CPP20_OR_LATER
        requires std::invocable<T>
#endif
    auto Finally(T&& func)
    {
        return FinalAction<std::decay_t<T>>(std::forward<std::decay_t<T>>(func));
    }
} // namespace nador

#endif //!__NADOR_FINALLY_H__