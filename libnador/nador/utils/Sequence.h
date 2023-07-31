#ifndef __NADOR_SEQUENCE_H__
#define __NADOR_SEQUENCE_H__

#include <functional>
#include <vector>
#include <chrono>

namespace nador
{
    template <typename ActionTy, typename DurationTy = std::chrono::milliseconds>
    struct ElementSequence
    {
        template<typename... Args>
        void operator()(Args&&... args) const
        {
            if(action)
            {
                action(std::forward<Args>(args)...);
            }
        }

        DurationTy duration{};
        ActionTy   action{};
    };

    template<typename ActionTy, typename DurationTy = std::chrono::milliseconds>
    class Sequence
    {
    public:
        Sequence(std::vector<ElementSequence<ActionTy, DurationTy>> seq)
        : _sequence(std::move(seq))
        {
        }

        void Start()
        {
            
        }

        void Stop()
        {

        }

        template<typenaem... Args>
        void Pull(Args&&... args)
        {
            for(const auto& it : _pendingActions)
            {
                it(args...);
            }

            _pendingActions.clear();
        }

    private:
        std::vector<ElementSequence<ActionTy, DurationTy>> _sequence;
        std::vector<ActionTy> _pendingActions;
    };

} // namespace nador

#endif //!__NADOR_SEQUENCE_H__