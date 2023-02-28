#ifndef __EVENT_H__
#define __EVENT_H__

#include <list>

#include "nador/utils/event/EventListener.h"

namespace nador
{
	template<typename... Args>
	class Event
	{
	public:
		using listener_t = EventListener<Args...>;
		using listener_list_t = std::list<listener_t*>;

		/*!
		 * Event destructor.
		 * Detach all event listeners if some is left.
		 */
		virtual ~Event()
		{
			for (auto it : _listeners)
			{
				it->DetachEvent(this);
			}
		}

		/*!
		 * Add listener to the event.
		 *
		 * \param eventListener    The event listener.
		 */
		void operator+=(listener_t* pEventListener)
		{
			auto findIter = std::find(_listeners.begin(), _listeners.end(), pEventListener);

			if (findIter != _listeners.end())
			{
				ENGINE_WARNING("Already registered listener to this event!");
				return;
			}

			_listeners.push_back(pEventListener);
			pEventListener->AttachEvent(this);
		}

		/*!
		 * Removes listener from the event.
		 *
		 * \param eventListener    The event listener.
		 */
		void operator-=(listener_t* pEventListener)
		{
			auto findIter = std::find(_listeners.begin(), _listeners.end(), pEventListener);

			if (findIter == _listeners.end())
			{
				ENGINE_WARNING("Not registered listener to this event!");
				return;
			}

			_listeners.remove(pEventListener);
			pEventListener->DetachEvent(this);
		}

		/*!
		 * Fire the event.
		 *
		 * \param args    The event arguments.
		 */
		void operator()(Args... args)
		{
			for(auto it : _listeners)
			{
				it->operator()(args...);
			}
		}

	protected:
		listener_list_t _listeners;
	};
}

#define CREATE_EVENT_TYPE_ARG_0(NAME) \
using NAME##_event_t = nador::Event<>; \
using NAME##_listener_t = nador::EventListener<>

#define CREATE_EVENT_TYPE_ARG_1(NAME, ARG_1) \
using NAME##_event_t = nador::Event<ARG_1>; \
using NAME##_listener_t = nador::EventListener<ARG_1>

#define CREATE_EVENT_TYPE_ARG_2(NAME, ARG_1, ARG_2) \
using NAME##_event_t = nador::Event<ARG_1, ARG_2>; \
using NAME##_listener_t = nador::EventListener<ARG_1, ARG_2>

#define CREATE_EVENT_TYPE_ARG_3(NAME, ARG_1, ARG_2, ARG_3) \
using NAME##_event_t = nador::Event<ARG_1, ARG_2, ARG_3>; \
using NAME##_listener_t = nador::EventListener<ARG_1, ARG_2, ARG_3>

#define CREATE_EVENT_TYPE_ARG_4(NAME, ARG_1, ARG_2, ARG_3, ARG_4) \
using NAME##_event_t = nador::Event<ARG_1, ARG_2, ARG_3, ARG_4>; \
using NAME##_listener_t = nador::EventListener<ARG_1, ARG_2, ARG_3, ARG_4>

#define CREATE_EVENT_TYPE_ARG_5(NAME, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5) \
using NAME##_event_t = nador::Event<ARG_1, ARG_2, ARG_3, ARG_4, ARG_5>; \
using NAME##_listener_t = nador::EventListener<ARG_1, ARG_2, ARG_3, ARG_4, ARG_5>

#endif // !__EVENT_H__
