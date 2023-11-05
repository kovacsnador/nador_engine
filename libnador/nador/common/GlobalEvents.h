#ifndef __GLOBAL_EVENTS_H__
#define __GLOBAL_EVENTS_H__

#include <chrono>

#include "nador/utils/event/Event.h"
#include "nador/common/KeyCodes.h"
#include "nador/common/Mouse.h"

namespace nador
{
	class IRenderer;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_0(onWindowClose);
	extern onWindowClose_event_t g_onWindowCloseEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_0(onAppStart);
	extern onAppStart_event_t g_onAppStartEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_0(onAppStop);
	extern onAppStop_event_t g_onAppStopEvent;

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
	CREATE_EVENT_TYPE_ARG_1(onAnimationTick, std::chrono::milliseconds);
	extern onAnimationTick_event_t g_onAnimationTickEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_0(onTickEnd);
	extern onTickEnd_event_t g_onTickEndEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_1(onRender, IRenderer*);
	extern onRender_event_t g_onRenderEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_1(onUiRender, IRenderer*);
	extern onUiRender_event_t g_onUiRenderEvent;

	//=====================================================================
	CREATE_EVENT_TYPE_ARG_1(onDebugRender, IRenderer*);
	extern onDebugRender_event_t g_onDebugRenderEvent;
}

#endif // !__GLOBAL_EVENTS_H__


