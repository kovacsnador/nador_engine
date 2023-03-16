#include "nador/App.h"
#include "nador/common/GlobalEvents.h"

namespace nador
{
	onWindowClose_event_t g_onWindowCloseEvent;

	onKey_event_t g_onKeyPressedEvent;
	onKey_event_t g_onKeyHoldedEvent;
	onKey_event_t g_onKeyReleasedEvent;

	onChar_event_t g_onCharEvent;

	onMouse_event_t g_onMousePressedEvent;
	onMouse_event_t g_onMouseReleasedEvent;

	onTickBegin_event_t g_onTickBeginEvent;
	onTick_event_t		g_onTickEvent;
	onTickEnd_event_t	g_onTickEndEvent;

	onRender_event_t		g_onRenderEvent;
	onDebugRender_event_t	g_onDebugRenderEvent;
}