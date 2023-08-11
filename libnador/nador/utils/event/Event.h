#ifndef __EVENT_H__
#define __EVENT_H__

#include <list>
#include <concepts>

#include "nador/utils/event/EventListener.h"

namespace nador
{
    template <typename... Args>
    class Event : private OnlyMoveable
    {
    public:
        using listener_t      = EventListener<Args...>;
        using listener_list_t = std::list<listener_t*>;

        /*!
         * Event default constructor.
         */
        Event() = default;

        /*!
         * Event default move constructor.
         */
        Event(Event&& other) noexcept
        : _listeners(other._listeners)
        {
            other._Detach();
            _Attach();
        }

        /*!
         * Event default move assign operator.
         */
        Event& operator=(Event&& other) noexcept
        {
            if (&other != this)
            {
                _listeners = other._listeners;

                other._Detach();
                _Attach();
            }
            return *this;
        }

        /*!
         * Event destructor.
         * Detach all event listeners if some is left.
         */
        ~Event() { _Detach(); }

        /*!
         * Add listener to the event.
         *
         * \param listener    The event listener.
         */
        void operator+=(listener_t& listener)
        {
            auto ok = _HandleListener(
                listener, [this](auto& l) { return _Attach(l); }, [](auto it, auto end) { return it == end; });

            if (ok == false)
            {
                ENGINE_WARNING("Already registered listener to this event!");
            }
        }

        /*!
         * Add listener to the event.
         *
         * \param listener    The event listener.
         */
        void PushFront(listener_t& listener)
        {
            auto ok = _HandleListener(
                listener,
                [this](auto& l) {
                    _listeners.push_front(&l);
                    l.AttachEvent(*this);
                    return true;
                },
                [](auto it, auto end) { return it == end; });

            if (ok == false)
            {
                ENGINE_WARNING("Already registered listener to this event!");
            }
        }

        /*!
         * Removes listener from the event.
         *
         * \param eventListener    The event listener.
         */
        void operator-=(listener_t& listener)
        {
            auto ok = _HandleListener(
                listener, [this](auto& l) { return _Detach(l); }, [](auto it, auto end) { return it != end; });

            if (ok == false)
            {
                ENGINE_WARNING("Not registered listener to this event!");
            }
        }

        /*!
         * Fire the event.
         *
         * \param args    The event arguments.
         */
        void operator()(Args... args)
        {
            for (auto it : _listeners)
            {
                it->operator()(args...);
            }
        }

    private:
        using iter_t = typename listener_list_t::iterator;

        template <typename CallbackTy, typename PredTy>
            requires requires(CallbackTy a, listener_t& b) {
                {
                    a(b)
                } -> std::same_as<bool>;
            } && std::predicate<PredTy, iter_t, iter_t>
        bool _HandleListener(listener_t& pEventListener, CallbackTy callback, PredTy pred)
        {
            auto findIter = std::find(_listeners.begin(), _listeners.end(), &pEventListener);

            if (pred(findIter, _listeners.end()))
            {
                return callback(pEventListener);
            }
            return false;
        }

        bool _Attach(listener_t& eventListener)
        {
            _listeners.push_back(&eventListener);
            eventListener.AttachEvent(*this);
            return true;
        }

        bool _Detach(listener_t& eventListener)
        {
            _listeners.remove(&eventListener);
            eventListener.DetachEvent(*this);
            return true;
        }

        void _Detach()
        {
            std::for_each(_listeners.begin(), _listeners.end(), [this](auto& it) { it->DetachEvent(*this); });
            _listeners.clear();
        }

        void _Attach()
        {
            std::for_each(_listeners.begin(), _listeners.end(), [this](auto it) { it->AttachEvent(*this); });
        }

        listener_list_t _listeners;
    };
} // namespace nador

#define CREATE_EVENT_TYPE_ARG_0(NAME)                                                                                                                \
    using NAME##_event_t    = nador::Event<>;                                                                                                        \
    using NAME##_listener_t = nador::EventListener<>

#define CREATE_EVENT_TYPE_ARG_1(NAME, ARG_1)                                                                                                         \
    using NAME##_event_t    = nador::Event<ARG_1>;                                                                                                   \
    using NAME##_listener_t = nador::EventListener<ARG_1>

#define CREATE_EVENT_TYPE_ARG_2(NAME, ARG_1, ARG_2)                                                                                                  \
    using NAME##_event_t    = nador::Event<ARG_1, ARG_2>;                                                                                            \
    using NAME##_listener_t = nador::EventListener<ARG_1, ARG_2>

#define CREATE_EVENT_TYPE_ARG_3(NAME, ARG_1, ARG_2, ARG_3)                                                                                           \
    using NAME##_event_t    = nador::Event<ARG_1, ARG_2, ARG_3>;                                                                                     \
    using NAME##_listener_t = nador::EventListener<ARG_1, ARG_2, ARG_3>

#define CREATE_EVENT_TYPE_ARG_4(NAME, ARG_1, ARG_2, ARG_3, ARG_4)                                                                                    \
    using NAME##_event_t    = nador::Event<ARG_1, ARG_2, ARG_3, ARG_4>;                                                                              \
    using NAME##_listener_t = nador::EventListener<ARG_1, ARG_2, ARG_3, ARG_4>

#define CREATE_EVENT_TYPE_ARG_5(NAME, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5)                                                                             \
    using NAME##_event_t    = nador::Event<ARG_1, ARG_2, ARG_3, ARG_4, ARG_5>;                                                                       \
    using NAME##_listener_t = nador::EventListener<ARG_1, ARG_2, ARG_3, ARG_4, ARG_5>

#endif // !__EVENT_H__
