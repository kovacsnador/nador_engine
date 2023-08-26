#ifndef __NADOR_EVENT_H__
#define __NADOR_EVENT_H__

#include <unordered_set>

#include "nador/utils/event/EventListener.h"

namespace nador
{
    template <typename... Args>
    class Event
    {
    public:
        using listener_t      = EventListener<Args...>;
        using listener_list_t = std::unordered_set<listener_t*>;

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
            other._DetachAllListeners();
            _AttachAllListeners();
        }

        /*!
         * Event default move assign operator.
         */
        Event& operator=(Event&& other) noexcept
        {
            if (&other != this)
            {
                _listeners = other._listeners;

                other._DetachAllListeners();
                _AttachAllListeners();
            }
            return *this;
        }

        /*!
         * Event destructor.
         * Detach all event listeners if some is left.
         */
        virtual ~Event() { _DetachAllListeners(); }

        /*!
         * Add listener to the event.
         *
         * \param listener    The event listener.
         */
        void operator+=(listener_t& listener)
        {
            auto [_, inserted] = _listeners.insert(&listener);
            if (inserted)
            {
                listener.AttachEvent(*this);
            }
            else
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
            auto erased = _listeners.erase(&listener);
            if (erased)
            {
                listener.DetachEvent(*this);
            }
            else
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
        void _DetachAllListeners()
        {
            std::for_each(_listeners.begin(), _listeners.end(), [this](auto& it) { it->DetachEvent(*this); });
            _listeners.clear();
        }

        void _AttachAllListeners()
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

#endif // !__NADOR_EVENT_H__
