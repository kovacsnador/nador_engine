#ifndef __NADOR_FINALLY_H__
#define __NADOR_FINALLY_H__

#include <utility>

#include "nador/utils/NonCopyable.h"

namespace nador
{
    template <typename T>
        requires std::invocable<T>
    class FinalAction : OnlyMoveable
    {
    public:
        explicit FinalAction(T&& func) noexcept
        : _func(std::forward<T>(func))
        {
        }

        FinalAction(FinalAction&& move) noexcept
        : _func(std::move(move._func))
        , _invoke(std::exchange(move._invoke, false))
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
        T    _func {};
        bool _invoke { true };
    };

    template <typename T>
        requires std::invocable<T>
    auto Finally(T&& func)
    {
        return FinalAction<T>(std::forward<T>(func));
    }
} // namespace nador

#endif //!__NADOR_FINALLY_H__