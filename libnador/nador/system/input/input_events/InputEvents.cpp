#include "nador/system/input/input_events/InputEvents.h"
#include "nador/common/GlobalEvents.h"
#include "nador/utils/ImGuiHelper.h"
#include "InputEvents.h"

/*bool nador::HandleInputEvent(InputEvents_t event)
{
    bool handledEvent = std::visit(
        [](auto&& e) -> bool {
            using T = std::decay_t<decltype(e)>;

            if constexpr (std::is_same_v<T, OnWindowClosedEvent>)
            {
                g_onWindowCloseEvent();
            }
            else if constexpr (std::is_same_v<T, OnKeyPressedEvent>)
            {
                if(IsInputKeyHandledByOthers())
                {
                    return false;    
                }

                g_onKeyPressedEvent(e.keyCode);
            }
            else if constexpr (std::is_same_v<T, OnKeyHoldedEvent>)
            {
                if(IsInputKeyHandledByOthers())
                {
                    return false;    
                }

                g_onKeyHoldedEvent(e.keyCode);
            }
            else if constexpr (std::is_same_v<T, OnKeyReleasedEvent>)
            {
                if(IsInputKeyHandledByOthers())
                {
                    return false;    
                }

                g_onKeyReleasedEvent(e.keyCode);
            }
            else if constexpr (std::is_same_v<T, OnCharEvent>)
            {   
                if(IsInputKeyHandledByOthers())
                {
                    return false;    
                }

                g_onCharEvent(e.multiChar);
            }
            else if constexpr (std::is_same_v<T, OnMousePressedEvent>)
            {
                if(IsInputMouseHandledByOthers())
                {
                    return false;
                }

                g_onMousePressedEvent(e.mouseButton, e.position);
            }
            else if constexpr (std::is_same_v<T, OnMouseReleasedEvent>)
            {
                if(IsInputMouseHandledByOthers())
                {
                    return false;
                }

                g_onMouseReleasedEvent(e.mouseButton, e.position);
            }
            else
            {
                return false;
            }
            return true;
        },
        event);

    return handledEvent;
}*/

bool nador::IsInputKeyHandledByOthers()
{
    return nador::Imgui_WantCaptureKeyboard();
}

bool nador::IsInputMouseHandledByOthers()
{
    return nador::Imgui_WantCaptureMouse();
}

bool nador::InputEventHandler::HandleInputEvent(InputEvents_t event) const
{
    bool handledEvent = std::visit(
        [this](auto&& e) -> bool {
            using T = std::decay_t<decltype(e)>;

            if constexpr (std::is_same_v<T, OnWindowClosedEvent>)
            {
                g_onWindowCloseEvent();
            }
            else if constexpr (std::is_same_v<T, OnKeyPressedEvent>)
            {
                if(IsInputKeyHandledByOthers())
                {
                    return false;    
                }

                g_onKeyPressedEvent(e.keyCode);
            }
            else if constexpr (std::is_same_v<T, OnKeyHoldedEvent>)
            {
                if(IsInputKeyHandledByOthers())
                {
                    return false;    
                }

                g_onKeyHoldedEvent(e.keyCode);
            }
            else if constexpr (std::is_same_v<T, OnKeyReleasedEvent>)
            {
                if(IsInputKeyHandledByOthers())
                {
                    return false;    
                }

                g_onKeyReleasedEvent(e.keyCode);
            }
            else if constexpr (std::is_same_v<T, OnCharEvent>)
            {   
                if(IsInputKeyHandledByOthers())
                {
                    return false;    
                }

                g_onCharEvent(e.multiChar);
            }
            else if constexpr (std::is_same_v<T, OnMousePressedEvent>)
            {
                if(IsInputMouseHandledByOthers())
                {
                    return false;
                }

                g_onMousePressedEvent(e.mouseButton, e.position);
            }
            else if constexpr (std::is_same_v<T, OnMouseReleasedEvent>)
            {
                if(IsInputMouseHandledByOthers())
                {
                    return false;
                }

                g_onMouseReleasedEvent(e.mouseButton, e.position);
            }
            else
            {
                return false;
            }
            return true;
        },
        event);

    return handledEvent;
}

bool nador::InputEventHandler::IsInputKeyHandledByOthers() const
{
    if(_handledKey)
    {
        return _handledKey();
    }
    return false;
}

bool nador::InputEventHandler::IsInputMouseHandledByOthers() const
{
    if(_handledMouse)
    {
        return _handledMouse();
    }
    return false;
}
