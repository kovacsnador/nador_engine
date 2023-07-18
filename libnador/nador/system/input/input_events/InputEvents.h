#ifndef __NADOR_INPUT_EVENTS_H__
#define __NADOR_INPUT_EVENTS_H__

#include <variant>
#include <functional>

#include "nador/common/KeyCodes.h"
#include "nador/common/Mouse.h"

namespace nador
{
    struct OnWindowClosedEvent
    {
    };

    struct OnKeyPressedEvent
    {
        EKeyCode keyCode;
    };

    struct OnKeyHoldedEvent
    {
        EKeyCode keyCode;
    };

    struct OnKeyReleasedEvent
    {
        EKeyCode keyCode;
    };

    struct OnCharEvent
    {
        std::string multiChar;
    };

    struct OnMousePressedEvent
    {
        EMouseButton mouseButton;
        glm::vec2    position;
    };

    struct OnMouseReleasedEvent
    {
        EMouseButton mouseButton;
        glm::vec2    position;
    };

    using InputEvents_t = std::variant<OnWindowClosedEvent,
                                       OnKeyPressedEvent,
                                       OnKeyHoldedEvent,
                                       OnKeyReleasedEvent,
                                       OnCharEvent,
                                       OnMousePressedEvent,
                                       OnMouseReleasedEvent>;

    class InputEventHandler
    {
    public:
        using callback_t = std::function<bool()>;

        InputEventHandler(callback_t handleKey, callback_t handleMouse)
        : _handledKey(handleKey)
        , _handledMouse(handleMouse)
        {
        }

        bool HandleInputEvent(InputEvents_t event) const;

        bool IsInputKeyHandledByOthers() const;
        bool IsInputMouseHandledByOthers() const;

    private:
        callback_t _handledKey{};
        callback_t _handledMouse{};
    };

    //bool HandleInputEvent(InputEvents_t event);

    bool IsInputKeyHandledByOthers();
    bool IsInputMouseHandledByOthers();

} // namespace nador

#endif //!__NADOR_INPUT_EVENTS_H__