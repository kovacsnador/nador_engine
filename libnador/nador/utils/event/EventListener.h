#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__

#include <functional>
#include <algorithm>
#include <list>

#include "nador/log/Log.h"

namespace nador
{
	namespace arg = std::placeholders;

	template<typename... Args> 
	class Event;

	template<typename... Args>
	class EventListener
	{
		using callback_t = std::function<void(Args...)>;
		using event_t = Event<Args...>;
		using event_list_t = std::list<event_t*>;

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
		 * \param pEvent		The event to attach.
		 * \param callback  The callback on event fire.
		 */
		EventListener(event_t* pEvent, const callback_t& callback)
		: _callback(callback)
		{
			pEvent->operator+=(this);
		}

		/*!
		 * EventListener constructor.
		 * Attachs listener to the event directly.
		 *
		 * \param pEvent		The event to attach.
		 * \param callback  The callback on event fire.
		 */
		EventListener(event_t* pEvent, callback_t&& callback)
		: _callback(std::move(callback))
		{
			pEvent->operator+=(this);
		}

		/*!
		 * EventListener destructor.
		 * 
		 * Cleans up all events.
		 */
		virtual ~EventListener()
		{
			event_list_t tempList = _attachedEvents;
			for (auto& it : tempList)
			{
				it->operator-=(this);
			}
		}

		/*!
		 * Sets the callback for the event listener.
		 *
		 * \param callback  The callback on event fire.
		 */
		void SetCallback(const callback_t& callback)
		{
			_callback = callback;
		}

		/*!
		 * Sets the callback for the event listener.
		 *
		 * \param callback  The callback on event fire.
		 */
		void SetCallback(callback_t&& callback) noexcept
		{
			_callback = std::move(callback);
		}

		/*!
		 * Call the callback on event fired.
		 *
		 * \param args  The callback argumants.
		 */
		template<typename... Types>
		void operator()(Types&&... args)
		{
			if(_callback && _suspended == false)
			{
				_callback(std::forward<Types>(args)...);
			}
		}

		/*!
		 * Attach to an event.
		 *
		 * \param pEvent  The attached event.
		 */
		void AttachEvent(event_t* pEvent)
		{
			NADOR_ASSERT(pEvent);

			auto findIter = std::find(_attachedEvents.begin(), _attachedEvents.end(), pEvent);

			if (findIter != _attachedEvents.end())
			{
				ENGINE_WARNING("Event listener already added to event!");
				return;
			}

			_attachedEvents.push_back(pEvent);
		}

		/*!
		 * Detach from an event.
		 *
		 * \param pEvent  The detached event.
		 */
		void DetachEvent(const event_t* pEvent)
		{
			NADOR_ASSERT(pEvent);
			
			auto findIter = std::find(_attachedEvents.begin(), _attachedEvents.end(), pEvent);

			if (findIter == _attachedEvents.end())
			{
				ENGINE_WARNING("Event listener already detached from event!");
				return;
			}

			_attachedEvents.erase(findIter);
		}

		void Suspend(bool suspend)
		{
			_suspended = suspend;
		}

	private:
		callback_t		_callback;
		event_list_t	_attachedEvents;

		bool _suspended{ false };
	};
}

#endif // !__EVENT_LISTENER_H__

