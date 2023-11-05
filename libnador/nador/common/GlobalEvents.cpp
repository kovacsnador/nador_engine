#include "nador/common/GlobalEvents.h"

namespace nador
{
    onWindowClose_event_t g_onWindowCloseEvent;
    
    onAppStart_event_t g_onAppStartEvent;
    onAppStop_event_t g_onAppStopEvent;

    onKey_event_t g_onKeyPressedEvent;
    onKey_event_t g_onKeyHoldedEvent;
    onKey_event_t g_onKeyReleasedEvent;

    onChar_event_t g_onCharEvent;

    onMouse_event_t g_onMousePressedEvent;
    onMouse_event_t g_onMouseReleasedEvent;

    onTickBegin_event_t     g_onTickBeginEvent;
    onAnimationTick_event_t g_onAnimationTickEvent;
    onTick_event_t          g_onTickEvent;
    onTickEnd_event_t       g_onTickEndEvent;

    onRender_event_t      g_onRenderEvent;
    onUiRender_event_t    g_onUiRenderEvent;
    onDebugRender_event_t g_onDebugRenderEvent;
} // namespace nador