#ifndef __NADOR_SEQUENCE_H__
#define __NADOR_SEQUENCE_H__

#include <functional>
#include <vector>
#include <chrono>

#include "nador/utils/event/Event.h"

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

    template <typename T>
    using ElementSequenceVec_t = std::vector<ElementSequence<std::chrono::milliseconds, T&>>;

    template <typename UserDataTy, typename DurationTy = std::chrono::milliseconds>
    class PackedSequence
    {
    public:
        enum class EPlayPolicy
        {
            NORMAL,
            FORCED,
        };

        using sequence_list_t = std::vector<ElementSequence<DurationTy, UserDataTy&>>;

        PackedSequence() = default;

        PackedSequence(UserDataTy& userData, sequence_list_t seq, Event<DurationTy>& event)
        : _userData(std::addressof(userData))
        , _sequence(std::move(seq))
        , _listener([this](DurationTy delta) { _Tick(delta, std::ref(*_userData)); })
        {
            _currentIter = _sequence.end();
            _listener.Suspend(true);
            event += _listener;
        }

        PackedSequence(PackedSequence&& other) noexcept
        : _userData(std::move(other._userData))
        , _listener(std::move(other._listener))
        , _deltaBuffer(std::move(other._deltaBuffer))
        {
            _listener.SetCallback([this](DurationTy delta) { _Tick(delta, std::ref(*_userData)); });

            auto idx = other._currentIter - other._sequence.begin();

            _sequence    = std::move(other._sequence);
            _currentIter = _sequence.begin() + idx;
        }

        PackedSequence& operator=(PackedSequence&& other) noexcept
        {
            _userData    = other._userData;
            _listener    = std::move(other._listener);
            _deltaBuffer = std::move(other._deltaBuffer);

            _listener.SetCallback([this](DurationTy delta) { _Tick(delta, std::ref(*_userData)); });

            auto idx = other._currentIter - other._sequence.begin();

            _sequence    = std::move(other._sequence);
            _currentIter = _sequence.begin() + idx;
            
            return *this;
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

        UserDataTy*               _userData { nullptr };
        sequence_list_t           _sequence;
        sequence_list_t::iterator _currentIter;

        EventListener<DurationTy> _listener;
        DurationTy                _deltaBuffer { 0 };
    };

    template <typename ObjTy, typename ContainterTy, typename FuncTy, typename DurationTy = std::chrono::milliseconds>
    constexpr auto CreateElemSequence(const ContainterTy& items, FuncTy func, DurationTy duration = 16ms)
    {
        std::vector<ElementSequence<DurationTy, ObjTy&>> result;

        DurationTy zero {};

        for (auto it = std::begin(items); it != std::end(items); ++it)
        {
            auto callback = [arg = *it, func](auto& obj) { std::invoke(func, obj, arg); };

            if (it != std::begin(items))
            {
                result.emplace_back(duration, callback);
            }
            else
            {
                result.emplace_back(zero, callback);
            }
        }
        return result;
    }

} // namespace nador

#endif //!__NADOR_SEQUENCE_H__