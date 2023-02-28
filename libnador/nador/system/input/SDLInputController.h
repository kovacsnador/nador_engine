#ifndef __INPUT_CONTROLLER_SDL_H__
#define __INPUT_CONTROLLER_SDL_H__

#include <unordered_map>
#include <set>

#include <SDL.h>

#include "nador/system/IInputController.h"

namespace nador
{
    class SDLInputController : public IInputController
    {
    public:
        SDLInputController(void* window);

        bool IsKeyPressed(EKeyCode keyCode) const override;
        bool IsKeyReleased(EKeyCode keyCode) const override;
        bool IsKeyHolded(EKeyCode keyCode) const override;

        bool IsMouseButtonPressed(EMouseButton buttonCode) const override;
        bool IsMouseButtonReleased(EMouseButton buttonCode) const override;
        bool IsMouseButtonHolded(EMouseButton buttonCode) const override;

        glm::vec2 GetMousePosition() const override;

        void TickBegin() override;

    private:
        void     _PullEvents();
        uint32_t _GetMouseState() const;

        void _OnKeyPressed(const SDL_Event& event);
        void _OnKeyReleased(const SDL_Event& event);
        void _OnKeyHolded(const SDL_Event& event);

        void _OnMousePressed(const SDL_Event& event);
        void _OnMouseReleased(const SDL_Event& event);

        enum class EKeyState
        {
            PRESS = 0,
            HOLD,
            RELEASE,
        };

        bool _HasKeyState(int32_t sdlKeyCode, EKeyState keyState) const;

        uint32_t _lastMouseState { 0 };

        std::unordered_map<int32_t, EKeyState> _keyStates;
        std::unordered_map<int32_t, uint64_t>  _mouseStates;

        SDL_Window* _window;
    };
} // namespace nador

#endif //!__INPUT_CONTROLLER_SDL_H__