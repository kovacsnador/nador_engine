#ifndef __NADOR_EVENT_LISTENER_H__
#define __NADOR_EVENT_LISTENER_H__

#include <functional>
#include <set>

#include "nador/log/ILog.h"
#include "nador/utils/NonCopyable.h"

namespace nador
{
    namespace arg = std::placeholders;

    template <typename... Args>
    class Event;

    template <typename... Args>
    class EventListener
    {
        using callback_t   = std::function<void(Args...)>;
        using event_t      = Event<Args...>;
        using event_list_t = std::set<event_t*>;

        friend class Event<Args...>;

    public:
        /*!
         * EventListener default constructor.
         */
        EventListener() = default;

        /*!
         * EventListener constructor.
         *
         * \param callback  The callback on event fire.
         */
        EventListener(const callback_t& callback)
        : _callback(callback)
        {
        }

        /*!
         * EventListener constructor.
         *
         * \param callback  The callback on event fire.
         */
        EventListener(callback_t&& callback) noexcept
        : _callback(std::move(callback))
        {
        }

        /*!
         * EventListener constructor.
         * Attachs listener to the event directly.
         *
         * \param event		The event to attach.
         * \param callback  The callback on event fire.
         */
        EventListener(event_t& event, const callback_t& callback)
        : _callback(callback)
        {
            event += *this;
        }

        /*!
         * EventListener constructor.
         * Attachs listener to the event directly.
         *
         * \param event		The event to attach.
         * \param callback  The callback on event fire.
         */
        EventListener(event_t& event, callback_t&& callback)
        : _callback(std::move(callback))
        {
            event += *this;
        }

        /*!
         * EventListener copy constructor.
         *
         * \param other		The other object to move.
         */
        EventListener(const EventListener& other) noexcept
        : _callback(other._callback)
        , _events(other._events)
        , _suspended(other._suspended)
        {
            for (auto& it : _events)
            {
                *it += *this;
            }
        }

        /*!
         * EventListener move constructor.
         *
         * \param other		The other object to move.
         */
        EventListener(EventListener&& other) noexcept
        : _callback(std::move(other._callback))
        , _events(other._events)
        , _suspended(std::move(other._suspended))
        {
            other.Detach();

            for (auto& it : _events)
            {
                *it += *this;
            }
        }

        /*!
         * EventListener copy assign operator.
         *
         * \param other		The other object to move.
         */
        EventListener& operator=(const EventListener& other) noexcept
        {
            if (&other != this)
            {
                _callback  = other._callback;
                _suspended = other._suspended;
                _events    = other._events;

                for (auto& it : _events)
                {
                    *it += *this;
                }
            }

			return *this;
        }

        /*!
         * EventListener move assign operator.
         *
         * \param other		The other object to move.
         */
        EventListener& operator=(EventListener&& other) noexcept
        {
            if (&other != this)
            {
                _callback  = std::move(other._callback);
                _suspended = std::move(other._suspended);
                _events    = other._events;
                other.Detach();

                for (auto& it : _events)
                {
                    *it += *this;
                }
            }

			return *this;
        }

        /*!
         * EventListener destructor.
         *
         * Cleans up all events.
         */
        virtual ~EventListener() { Detach(); }

        /*!
         * Sets the callback for the event listener.
         *
         * \param callback  The callback on event fire.
         */
        void SetCallback(const callback_t& callback) { _callback = callback; }

        /*!
         * Sets the callback for the event listener.
         *
         * \param callback  The callback on event fire.
         */
        void SetCallback(callback_t&& callback) noexcept { _callback = std::move(callback); }

        /*!
         * Call the callback on event fired.
         *
         * \param args  The callback argumants.
         */
        template <typename... Types>
        void operator()(Types&&... args)
        {
            if (_callback && _suspended == false)
            {
                _callback(std::forward<Types>(args)...);
            }
        }

        /*!
         * Detach from all registered events.
         */
        void Detach()
        {
            event_list_t tempList = _events;
            for (auto& it : tempList)
            {
                it->operator-=(*this);
            }

            _events.clear();
        }

        void Suspend(bool suspend) { _suspended = suspend; }

    private:
        /*!
         * Attach to an event.
         *
         * \param event  The attached event.
         */
        bool AttachEvent(event_t& event)
        {
            auto [_, inserted] = _events.insert(&event);
            return inserted;
        }

        /*!
         * Detach from an event.
         *
         * \param event  The detached event.
         */
        bool DetachEvent(event_t& event)
        {
            _events.erase(&event);
            return true;
        }

        callback_t   _callback;
        event_list_t _events;

        bool _suspended { false };
    };

    // Deduction guide
    template<typename... Args>
    EventListener(std::function<void(Args...)>) -> EventListener<Args...>;

    template<typename... Args, typename T>
    EventListener(Event<Args...>, T) -> EventListener<Args...>;

} // namespace nador

#endif // !__NADOR_EVENT_LISTENER_H__
