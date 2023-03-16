#ifndef __GLOBAL_EVENTS_H__
#define __GLOBAL_EVENTS_H__

#include "nador/utils/event/Event.h"
#include "nador/common/KeyCodes.h"
#include "nador/common/Mouse.h"

namespace nador
{
	//=====================================================================
	CREATE_EVENT_TYPE_ARG_0(onWindowClose);
	extern onWindowClose_event_t g_onWindowCloseEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_1(onKey, EKeyCode);
	extern onKey_event_t g_onKeyPressedEvent;
	extern onKey_event_t g_onKeyHoldedEvent;
	extern onKey_event_t g_onKeyReleasedEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_1(onChar, const std::string&);
	extern onChar_event_t g_onCharEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_2(onMouse, EMouseButton, const glm::vec2&);
	extern onMouse_event_t g_onMousePressedEvent;
	extern onMouse_event_t g_onMouseReleasedEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_0(onTickBegin);
	extern onTickBegin_event_t g_onTickBeginEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_1(onTick, float_t);
	extern onTick_event_t g_onTickEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_0(onTickEnd);
	extern onTickEnd_event_t g_onTickEndEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_0(onRender);
	extern onRender_event_t g_onRenderEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_0(onDebugRender);
	extern onDebugRender_event_t g_onDebugRenderEvent;
}

#endif // !__GLOBAL_EVENTS_H__


