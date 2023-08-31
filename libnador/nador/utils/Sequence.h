#ifndef __NADOR_SEQUENCE_H__
#define __NADOR_SEQUENCE_H__

#include <functional>
#include <vector>
#include <chrono>

#include "nador/utils/event/Event.h"
#include "nador/utils/Time.h"

using namespace std::chrono_literals;

namespace nador
{
    template <typename DurationTy = std::chrono::milliseconds, typename... Args>
    struct ElementSequence
    {
        void operator()(Args&&... args) const
        {
            if (action)
            {
                action(std::forward<Args>(args)...);
            }
        }

        DurationTy                   duration {};
        std::function<void(Args...)> action {};
    };

    template<typename T>
    using ElementSequenceVec_t = std::vector<ElementSequence<std::chrono::milliseconds, T&>>;

    template <typename UserDataTy, typename DurationTy = std::chrono::milliseconds>
    class Sequence
    {
    public:
        enum class EPlayPolicy
        {
            NORMAL,
            FORCED,
        };

        using sequence_list_t = std::vector<ElementSequence<DurationTy, UserDataTy&>>;

        Sequence(UserDataTy& userData, sequence_list_t seq, Event<DurationTy>& event)
        : _sequence(std::move(seq))
        , _listener([this, &userData](DurationTy delta) { _Tick(delta, std::ref(userData)); })
        {
            _currentIter = _sequence.end();
            _listener.Suspend(true);
            event += _listener;
        }

        void Play(EPlayPolicy policy = EPlayPolicy::NORMAL)
        {
            _listener.Suspend(false);
            if (_currentIter == _sequence.end() || policy == EPlayPolicy::FORCED)
            {
                _deltaBuffer = DurationTy { 0 };
                _currentIter = _sequence.begin();
            }
        }

        void Pause() { _listener.Suspend(true); }

        void Stop()
        {
            _listener.Suspend(true);
            _currentIter = _sequence.end();
        }

    private:
        template <typename... Args>
        void _Tick(DurationTy deltaTime, Args... args)
        {
            _deltaBuffer += deltaTime;

            for (; _currentIter != _sequence.end(); ++_currentIter)
            {
                if (_deltaBuffer < _currentIter->duration)
                {
                    break;
                }

                // trigger sequence callback
                (*_currentIter)(args...);
                _deltaBuffer -= _currentIter->duration;
            }

            if (_currentIter == _sequence.end())
            {
                _listener.Suspend(true);
            }
        }

        //UserDataTy&               _userData;
        sequence_list_t           _sequence;
        sequence_list_t::iterator _currentIter;

        EventListener<DurationTy> _listener;
        DurationTy                _deltaBuffer { 0 };
    };
} // namespace nador

#endif //!__NADOR_SEQUENCE_H__