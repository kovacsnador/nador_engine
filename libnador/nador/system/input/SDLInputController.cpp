#include "imgui.h"
#include "backends/imgui_impl_sdl.h"

#include <SDL.h>

#include "nador/log/Log.h"

#include "nador/system/input/SDLInputController.h"
#include "nador/system/input/input_events/InputEvents.h"

namespace nador
{
    constexpr uint64_t MOUSE_HOLD_DELAY_MS = 500;

    static EMouseButton ConvertToNadorMouseButton(int32_t mouseButton)
    {
        switch (mouseButton)
        {
        case SDL_BUTTON_LEFT:
            return EMouseButton::LEFT;
        case SDL_BUTTON_RIGHT:
            return EMouseButton::RIGHT;
        case SDL_BUTTON_MIDDLE:
            return EMouseButton::MIDDLE;
        default:
            ENGINE_WARNING("Unknow mouse event");
            break;
        }
        return EMouseButton::UNKNOWN;
    }

    static int32_t ConvertToNativeMouseButton(EMouseButton mouseButton)
    {
        switch (mouseButton)
        {
        case EMouseButton::LEFT:
            return SDL_BUTTON_LEFT;
        case EMouseButton::RIGHT:
            return SDL_BUTTON_RIGHT;
        case EMouseButton::MIDDLE:
            return SDL_BUTTON_MIDDLE;
        default:
            ENGINE_WARNING("Unknow mouse event");
            break;
        }
        return SDL_BUTTON_LEFT; // return left as default
    }

    static EKeyCode ConvertToNadorKeyCode(int32_t keyCode)
    {
        switch (keyCode)
        {
        case SDLK_UNKNOWN:
            return EKeyCode::UNKNOWN;
        case SDLK_SPACE:
            return EKeyCode::SPACE;
        case SDLK_QUOTE:
            return EKeyCode::APOSTROPHE;
        case SDLK_COMMA:
            return EKeyCode::COMMA;
        case SDLK_MINUS:
            return EKeyCode::MINUS;
        case SDLK_PERIOD:
            return EKeyCode::PERIOD;
        case SDLK_SLASH:
            return EKeyCode::SLASH;
        case SDLK_0:
            return EKeyCode::NUM_0;
        case SDLK_1:
            return EKeyCode::NUM_1;
        case SDLK_2:
            return EKeyCode::NUM_2;
        case SDLK_3:
            return EKeyCode::NUM_3;
        case SDLK_4:
            return EKeyCode::NUM_4;
        case SDLK_5:
            return EKeyCode::NUM_5;
        case SDLK_6:
            return EKeyCode::NUM_6;
        case SDLK_7:
            return EKeyCode::NUM_7;
        case SDLK_8:
            return EKeyCode::NUM_8;
        case SDLK_9:
            return EKeyCode::NUM_9;
        case SDLK_SEMICOLON:
            return EKeyCode::SEMICOLON;
        case SDLK_EQUALS:
            return EKeyCode::EQUAL;
        case SDLK_a:
            return EKeyCode::A;
        case SDLK_b:
            return EKeyCode::B;
        case SDLK_c:
            return EKeyCode::C;
        case SDLK_d:
            return EKeyCode::D;
        case SDLK_e:
            return EKeyCode::E;
        case SDLK_f:
            return EKeyCode::F;
        case SDLK_g:
            return EKeyCode::G;
        case SDLK_h:
            return EKeyCode::H;
        case SDLK_i:
            return EKeyCode::I;
        case SDLK_j:
            return EKeyCode::J;
        case SDLK_k:
            return EKeyCode::K;
        case SDLK_l:
            return EKeyCode::L;
        case SDLK_m:
            return EKeyCode::M;
        case SDLK_n:
            return EKeyCode::N;
        case SDLK_o:
            return EKeyCode::O;
        case SDLK_p:
            return EKeyCode::P;
        case SDLK_q:
            return EKeyCode::Q;
        case SDLK_r:
            return EKeyCode::R;
        case SDLK_s:
            return EKeyCode::S;
        case SDLK_t:
            return EKeyCode::T;
        case SDLK_u:
            return EKeyCode::U;
        case SDLK_v:
            return EKeyCode::V;
        case SDLK_w:
            return EKeyCode::W;
        case SDLK_x:
            return EKeyCode::X;
        case SDLK_y:
            return EKeyCode::Y;
        case SDLK_z:
            return EKeyCode::Z;
        case SDLK_LEFTBRACKET:
            return EKeyCode::LEFT_BRACKET;
        case SDLK_BACKSLASH:
            return EKeyCode::BACKSLASH;
        case SDLK_RIGHTBRACKET:
            return EKeyCode::RIGHT_BRACKET;
        case SDLK_BACKQUOTE:
            return EKeyCode::GRAVE_ACCENT;
            /*case EKeyCode::WORLD_1: //TODO: check
                case SDLK_UNKNOWN: return
            case EKeyCode::WORLD_2: //TODO: check
                case SDLK_UNKNOWN: return*/

            /* Function keys */
        case SDLK_ESCAPE:
            return EKeyCode::ESCAPE;
        case SDLK_RETURN:
            return EKeyCode::ENTER;
        case SDLK_TAB:
            return EKeyCode::TAB;
        case SDLK_BACKSPACE:
            return EKeyCode::BACKSPACE;
        case SDLK_INSERT:
            return EKeyCode::INSERT;
        case SDLK_DELETE:
            return EKeyCode::DELETE;
        case SDLK_RIGHT:
            return EKeyCode::RIGHT;
        case SDLK_LEFT:
            return EKeyCode::LEFT;
        case SDLK_DOWN:
            return EKeyCode::DOWN;
        case SDLK_UP:
            return EKeyCode::UP;
        case SDLK_PAGEUP:
            return EKeyCode::PAGE_UP;
        case SDLK_PAGEDOWN:
            return EKeyCode::PAGE_DOWN;
        case SDLK_HOME:
            return EKeyCode::HOME;
        case SDLK_END:
            return EKeyCode::END;
        case SDLK_CAPSLOCK:
            return EKeyCode::CAPS_LOCK;
        case SDLK_SCROLLLOCK:
            return EKeyCode::SCROLL_LOCK;
        case SDLK_NUMLOCKCLEAR:
            return EKeyCode::NUM_LOCK;
        case SDLK_PRINTSCREEN:
            return EKeyCode::PRINT_SCREEN;
        case SDLK_PAUSE:
            return EKeyCode::PAUSE;

        case SDLK_F1:
            return EKeyCode::F1;
        case SDLK_F2:
            return EKeyCode::F2;
        case SDLK_F3:
            return EKeyCode::F3;
        case SDLK_F4:
            return EKeyCode::F4;
        case SDLK_F5:
            return EKeyCode::F5;
        case SDLK_F6:
            return EKeyCode::F6;
        case SDLK_F7:
            return EKeyCode::F7;
        case SDLK_F8:
            return EKeyCode::F8;
        case SDLK_F9:
            return EKeyCode::F9;
        case SDLK_F10:
            return EKeyCode::F10;
        case SDLK_F11:
            return EKeyCode::F11;
        case SDLK_F12:
            return EKeyCode::F12;
        case SDLK_F13:
            return EKeyCode::F13;
        case SDLK_F14:
            return EKeyCode::F14;
        case SDLK_F15:
            return EKeyCode::F15;
        case SDLK_F16:
            return EKeyCode::F16;
        case SDLK_F17:
            return EKeyCode::F17;
        case SDLK_F18:
            return EKeyCode::F18;
        case SDLK_F19:
            return EKeyCode::F19;
        case SDLK_F20:
            return EKeyCode::F20;
        case SDLK_F21:
            return EKeyCode::F21;
        case SDLK_F22:
            return EKeyCode::F22;
        case SDLK_F23:
            return EKeyCode::F23;
        case SDLK_F24:
            return EKeyCode::F24;
            // case SDLK_F25:
            //     return EKeyCode::F25;

        case SDLK_KP_0:
            return EKeyCode::KP_0;
        case SDLK_KP_1:
            return EKeyCode::KP_1;
        case SDLK_KP_2:
            return EKeyCode::KP_2;
        case SDLK_KP_3:
            return EKeyCode::KP_3;
        case SDLK_KP_4:
            return EKeyCode::KP_4;
        case SDLK_KP_5:
            return EKeyCode::KP_5;
        case SDLK_KP_6:
            return EKeyCode::KP_6;
        case SDLK_KP_7:
            return EKeyCode::KP_7;
        case SDLK_KP_8:
            return EKeyCode::KP_8;
        case SDLK_KP_9:
            return EKeyCode::KP_9;

        case SDLK_KP_DECIMAL:
            return EKeyCode::KP_DECIMAL;
        case SDLK_KP_DIVIDE:
            return EKeyCode::KP_DIVIDE;
        case SDLK_KP_MULTIPLY:
            return EKeyCode::KP_MULTIPLY;
        case SDLK_KP_MEMSUBTRACT:
            return EKeyCode::KP_SUBTRACT;
        case SDLK_KP_MEMADD:
            return EKeyCode::KP_ADD;
        case SDLK_KP_ENTER:
            return EKeyCode::KP_ENTER;
        case SDLK_KP_EQUALS:
            return EKeyCode::KP_EQUAL;
        case SDLK_LSHIFT:
            return EKeyCode::LEFT_SHIFT;
        case SDLK_LCTRL:
            return EKeyCode::LEFT_CONTROL;
        case SDLK_LALT:
            return EKeyCode::LEFT_ALT;
        case SDLK_LGUI:
            return EKeyCode::LEFT_SUPER; // : returODO: check
        case SDLK_RSHIFT:
            return EKeyCode::RIGHT_SHIFT;
        case SDLK_RCTRL:
            return EKeyCode::RIGHT_CONTROL;
        case SDLK_RALT:
            return EKeyCode::RIGHT_ALT;
        case SDLK_RGUI:
            return EKeyCode::RIGHT_SUPER; // : rODO: check
        case SDLK_MENU:
            return EKeyCode::MENU;

        default:
            return EKeyCode::UNKNOWN;
        }
    }

    static SDL_KeyCode ConvertToApiKeyCode(EKeyCode keyCode)
    {
        switch (keyCode)
        {
        case EKeyCode::UNKNOWN:
            return SDLK_UNKNOWN;
        case EKeyCode::SPACE:
            return SDLK_SPACE;
        case EKeyCode::APOSTROPHE:
            return SDLK_QUOTE;
        case EKeyCode::COMMA:
            return SDLK_COMMA;
        case EKeyCode::MINUS:
            return SDLK_MINUS;
        case EKeyCode::PERIOD:
            return SDLK_PERIOD;
        case EKeyCode::SLASH:
            return SDLK_SLASH;
        case EKeyCode::NUM_0:
            return SDLK_0;
        case EKeyCode::NUM_1:
            return SDLK_1;
        case EKeyCode::NUM_2:
            return SDLK_2;
        case EKeyCode::NUM_3:
            return SDLK_3;
        case EKeyCode::NUM_4:
            return SDLK_4;
        case EKeyCode::NUM_5:
            return SDLK_5;
        case EKeyCode::NUM_6:
            return SDLK_6;
        case EKeyCode::NUM_7:
            return SDLK_7;
        case EKeyCode::NUM_8:
            return SDLK_8;
        case EKeyCode::NUM_9:
            return SDLK_9;
        case EKeyCode::SEMICOLON:
            return SDLK_SEMICOLON;
        case EKeyCode::EQUAL:
            return SDLK_EQUALS;
        case EKeyCode::A:
            return SDLK_a;
        case EKeyCode::B:
            return SDLK_b;
        case EKeyCode::C:
            return SDLK_c;
        case EKeyCode::D:
            return SDLK_d;
        case EKeyCode::E:
            return SDLK_e;
        case EKeyCode::F:
            return SDLK_f;
        case EKeyCode::G:
            return SDLK_g;
        case EKeyCode::H:
            return SDLK_h;
        case EKeyCode::I:
            return SDLK_i;
        case EKeyCode::J:
            return SDLK_j;
        case EKeyCode::K:
            return SDLK_k;
        case EKeyCode::L:
            return SDLK_l;
        case EKeyCode::M:
            return SDLK_m;
        case EKeyCode::N:
            return SDLK_n;
        case EKeyCode::O:
            return SDLK_o;
        case EKeyCode::P:
            return SDLK_p;
        case EKeyCode::Q:
            return SDLK_q;
        case EKeyCode::R:
            return SDLK_r;
        case EKeyCode::S:
            return SDLK_s;
        case EKeyCode::T:
            return SDLK_t;
        case EKeyCode::U:
            return SDLK_u;
        case EKeyCode::V:
            return SDLK_v;
        case EKeyCode::W:
            return SDLK_w;
        case EKeyCode::X:
            return SDLK_x;
        case EKeyCode::Y:
            return SDLK_y;
        case EKeyCode::Z:
            return SDLK_z;
        case EKeyCode::LEFT_BRACKET:
            return SDLK_LEFTBRACKET;
        case EKeyCode::BACKSLASH:
            return SDLK_BACKSLASH;
        case EKeyCode::RIGHT_BRACKET:
            return SDLK_RIGHTBRACKET;
        case EKeyCode::GRAVE_ACCENT:
            return SDLK_BACKQUOTE;
        case EKeyCode::WORLD_1:
            return SDLK_UNKNOWN; // TODO: check
        case EKeyCode::WORLD_2:
            return SDLK_UNKNOWN; // TODO: check

            /* Function keys */
        case EKeyCode::ESCAPE:
            return SDLK_ESCAPE;
        case EKeyCode::ENTER:
            return SDLK_RETURN;
        case EKeyCode::TAB:
            return SDLK_TAB;
        case EKeyCode::BACKSPACE:
            return SDLK_BACKSPACE;
        case EKeyCode::INSERT:
            return SDLK_INSERT;
        case EKeyCode::DELETE:
            return SDLK_DELETE;
        case EKeyCode::RIGHT:
            return SDLK_RIGHT;
        case EKeyCode::LEFT:
            return SDLK_LEFT;
        case EKeyCode::DOWN:
            return SDLK_DOWN;
        case EKeyCode::UP:
            return SDLK_UP;
        case EKeyCode::PAGE_UP:
            return SDLK_PAGEUP;
        case EKeyCode::PAGE_DOWN:
            return SDLK_PAGEDOWN;
        case EKeyCode::HOME:
            return SDLK_HOME;
        case EKeyCode::END:
            return SDLK_END;
        case EKeyCode::CAPS_LOCK:
            return SDLK_CAPSLOCK;
        case EKeyCode::SCROLL_LOCK:
            return SDLK_SCROLLLOCK;
        case EKeyCode::NUM_LOCK:
            return SDLK_NUMLOCKCLEAR;
        case EKeyCode::PRINT_SCREEN:
            return SDLK_PRINTSCREEN;
        case EKeyCode::PAUSE:
            return SDLK_PAUSE;

        case EKeyCode::F1:
            return SDLK_F1;
        case EKeyCode::F2:
            return SDLK_F2;
        case EKeyCode::F3:
            return SDLK_F3;
        case EKeyCode::F4:
            return SDLK_F4;
        case EKeyCode::F5:
            return SDLK_F5;
        case EKeyCode::F6:
            return SDLK_F6;
        case EKeyCode::F7:
            return SDLK_F7;
        case EKeyCode::F8:
            return SDLK_F8;
        case EKeyCode::F9:
            return SDLK_F9;
        case EKeyCode::F10:
            return SDLK_F10;
        case EKeyCode::F11:
            return SDLK_F11;
        case EKeyCode::F12:
            return SDLK_F12;
        case EKeyCode::F13:
            return SDLK_F13;
        case EKeyCode::F14:
            return SDLK_F14;
        case EKeyCode::F15:
            return SDLK_F15;
        case EKeyCode::F16:
            return SDLK_F16;
        case EKeyCode::F17:
            return SDLK_F17;
        case EKeyCode::F18:
            return SDLK_F18;
        case EKeyCode::F19:
            return SDLK_F19;
        case EKeyCode::F20:
            return SDLK_F20;
        case EKeyCode::F21:
            return SDLK_F21;
        case EKeyCode::F22:
            return SDLK_F22;
        case EKeyCode::F23:
            return SDLK_F23;
        case EKeyCode::F24:
            return SDLK_F24;
        case EKeyCode::F25:
            return SDLK_UNKNOWN;

        case EKeyCode::KP_0:
            return SDLK_KP_0;
        case EKeyCode::KP_1:
            return SDLK_KP_1;
        case EKeyCode::KP_2:
            return SDLK_KP_2;
        case EKeyCode::KP_3:
            return SDLK_KP_3;
        case EKeyCode::KP_4:
            return SDLK_KP_4;
        case EKeyCode::KP_5:
            return SDLK_KP_5;
        case EKeyCode::KP_6:
            return SDLK_KP_6;
        case EKeyCode::KP_7:
            return SDLK_KP_7;
        case EKeyCode::KP_8:
            return SDLK_KP_8;
        case EKeyCode::KP_9:
            return SDLK_KP_9;

        case EKeyCode::KP_DECIMAL:
            return SDLK_KP_DECIMAL;
        case EKeyCode::KP_DIVIDE:
            return SDLK_KP_DIVIDE;
        case EKeyCode::KP_MULTIPLY:
            return SDLK_KP_MULTIPLY;
        case EKeyCode::KP_SUBTRACT:
            return SDLK_KP_MEMSUBTRACT;
        case EKeyCode::KP_ADD:
            return SDLK_KP_MEMADD;
        case EKeyCode::KP_ENTER:
            return SDLK_KP_ENTER;
        case EKeyCode::KP_EQUAL:
            return SDLK_KP_EQUALS;
        case EKeyCode::LEFT_SHIFT:
            return SDLK_LSHIFT;
        case EKeyCode::LEFT_CONTROL:
            return SDLK_LCTRL;
        case EKeyCode::LEFT_ALT:
            return SDLK_LALT;
        case EKeyCode::LEFT_SUPER:
            return SDLK_LGUI; // TODO: check
        case EKeyCode::RIGHT_SHIFT:
            return SDLK_RSHIFT;
        case EKeyCode::RIGHT_CONTROL:
            return SDLK_RCTRL;
        case EKeyCode::RIGHT_ALT:
            return SDLK_RALT;
        case EKeyCode::RIGHT_SUPER:
            return SDLK_RGUI; // TODO: check
        case EKeyCode::MENU:
            return SDLK_MENU;
        default:
            return SDLK_UNKNOWN;
        }
    }

    SDLInputController::SDLInputController(void* window, InputEventHandler inputHandler)
    : _window(static_cast<SDL_Window*>(window))
    , _inputHandler(std::move(inputHandler))
    {
    }

    void SDLInputController::_OnKeyPressed(const SDL_Event& event)
    {
        SDL_Keycode keyCode = event.key.keysym.sym;
        _keyStates.insert_or_assign(keyCode, EKeyState::PRESS);
        _inputHandler.HandleInputEvent(OnKeyPressedEvent{ConvertToNadorKeyCode(keyCode)});
    }

    void SDLInputController::_OnKeyReleased(const SDL_Event& event)
    {
        SDL_Keycode keyCode = event.key.keysym.sym;
        _keyStates.insert_or_assign(keyCode, EKeyState::RELEASE);
        _inputHandler.HandleInputEvent(OnKeyReleasedEvent{ConvertToNadorKeyCode(keyCode)});
    }

    void SDLInputController::_OnKeyHolded(const SDL_Event& event)
    {
        SDL_Keycode keyCode = event.key.keysym.sym;
        _keyStates.insert_or_assign(keyCode, EKeyState::HOLD);
        _inputHandler.HandleInputEvent(OnKeyHoldedEvent{ConvertToNadorKeyCode(keyCode)});
    }

    void SDLInputController::_OnMousePressed(const SDL_Event& event)
    {
        auto sdlButton = event.button;

        _mouseStates.insert_or_assign(sdlButton.button, SDL_GetTicks64());
        _inputHandler.HandleInputEvent(OnMousePressedEvent{ConvertToNadorMouseButton(sdlButton.button), GetMousePosition()});
    }

    void SDLInputController::_OnMouseReleased(const SDL_Event& event)
    {
        auto sdlButton = event.button;

        _mouseStates.erase(sdlButton.button);
        _inputHandler.HandleInputEvent(OnMouseReleasedEvent{ConvertToNadorMouseButton(sdlButton.button), GetMousePosition()});
    }

    bool SDLInputController::IsKeyPressed(EKeyCode keyCode) const
    {
        return _HasKeyState(ConvertToApiKeyCode(keyCode), EKeyState::PRESS);
    }

    bool SDLInputController::IsKeyReleased(EKeyCode keyCode) const
    {
        return _HasKeyState(ConvertToApiKeyCode(keyCode), EKeyState::RELEASE);
    }

    bool SDLInputController::IsKeyHolded(EKeyCode keyCode) const
    {
        return _HasKeyState(ConvertToApiKeyCode(keyCode), EKeyState::HOLD);
    }

    bool SDLInputController::_HasKeyState(int32_t sdlKeyCode, EKeyState keyState) const
    {
        if(_inputHandler.IsInputKeyHandledByOthers())
        {
            return false;
        }

        if (auto find = _keyStates.find(sdlKeyCode); find != _keyStates.end())
        {
            return find->second == keyState;
        }
        return false;
    }

    bool SDLInputController::IsMouseButtonPressed(EMouseButton buttonCode) const
    {
        if(_inputHandler.IsInputMouseHandledByOthers())
        {
            return false;
        }

        auto sdlButtonMask = SDL_BUTTON(ConvertToNativeMouseButton(buttonCode));

        uint32_t state = _GetMouseState();
        return (state & sdlButtonMask);
    }

    bool SDLInputController::IsMouseButtonReleased(EMouseButton buttonCode) const
    {
        if(_inputHandler.IsInputMouseHandledByOthers())
        {
            return false;
        }

        auto sdlButtonMask = SDL_BUTTON(ConvertToNativeMouseButton(buttonCode));

        if (_lastMouseState & sdlButtonMask)
        {
            uint32_t state = _GetMouseState();
            return !(state & sdlButtonMask);
        }

        return false;
    }

    bool SDLInputController::IsMouseButtonHolded(EMouseButton buttonCode) const
    {
        if(_inputHandler.IsInputMouseHandledByOthers())
        {
            return false;
        }

        if (auto find = _mouseStates.find(ConvertToNativeMouseButton(buttonCode)); find != _mouseStates.end())
        {
            return (SDL_GetTicks64() - find->second) > MOUSE_HOLD_DELAY_MS;
        }
        return false;
    }

    glm::vec2 SDLInputController::GetMousePosition() const
    {
        if (_inputHandler.IsInputMouseHandledByOthers())
        {
            return {-1, -1};
        }

        NADOR_ASSERT(_window);

        int32_t x;
        int32_t y;
        SDL_GetMouseState(&x, &y);

        int32_t width;
        int32_t height;
        SDL_GetWindowSize(_window, &width, &height);

        // flip the y axis
        y = (double_t)height - y;

        return { (float)x, (float)y };
    }

    void SDLInputController::TickBegin()
    {
        _PullEvents();
    }

    void SDLInputController::_PullEvents()
    {
        _lastMouseState = _GetMouseState();
        _keyStates.clear();

        SDL_Event event;
        while (SDL_PollEvent(&event)) // poll until all events are handled!
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch (event.type)
            {
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    _inputHandler.HandleInputEvent(OnWindowClosedEvent{});
                }
                break;
            case SDL_QUIT:
                _inputHandler.HandleInputEvent(OnWindowClosedEvent{});
                break;
            case SDL_KEYDOWN: {
                if (event.key.repeat == 0)
                {
                    _OnKeyPressed(event);
                }
                else
                {
                    _OnKeyHolded(event);
                }
                break;
            }
            case SDL_KEYUP:
                _OnKeyReleased(event);
                break;
            case SDL_TEXTINPUT:
                _inputHandler.HandleInputEvent(OnCharEvent{event.text.text});
                break;
            case SDL_MOUSEBUTTONDOWN:
                _OnMousePressed(event);
                break;
            case SDL_MOUSEBUTTONUP:
                _OnMouseReleased(event);
                break;
            default:
                // ENGINE_WARNING("SDL Event not handled. Type: %d", event.type);
                break;
            }
        }
    }

    uint32_t SDLInputController::_GetMouseState() const
    {
        int32_t x;
        int32_t y;
        return SDL_GetMouseState(&x, &y);
    }

} // namespace nador