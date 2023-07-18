#include <string>
#include <codecvt>
#include <locale>

#include <GLFW/glfw3.h>

#include "nador/system/input/GLFWInputController.h"
#include "nador/log/Log.h"

#include "nador/system/input/input_events/InputEvents.h"
#include "nador/utils/event/Event.h"

namespace nador
{
    static Event<InputEvents_t> s_glfwInputEvent;

    static glm::vec2 GetCursorPosition(GLFWwindow* window)
    {
        NADOR_ASSERT(window);

        double_t x;
        double_t y;

        glfwGetCursorPos(window, &x, &y);

        int32_t width;
        int32_t height;
        glfwGetWindowSize(window, &width, &height);

        // flip the y axis
        y = (double_t)height - y;

        return { (float)x, (float)y };
    }

    static EKeyCode ConvertToNadorKeyCode(int keyCode)
    {
        return static_cast<EKeyCode>(keyCode);
    }

    static int32_t ConvertToApiKeyCode(EKeyCode keyCode)
    {
        return static_cast<int32_t>(keyCode);
    }

    static void Glfw_KeyCallback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
    {
        EKeyCode keycode = ConvertToNadorKeyCode(key);

        switch (action)
        {
        case GLFW_PRESS:
            //HandleInputEvent(OnKeyPressedEvent { keycode });
            s_glfwInputEvent(OnKeyPressedEvent { keycode });
            break;
        case GLFW_REPEAT:
            //HandleInputEvent(OnKeyHoldedEvent { keycode });
            s_glfwInputEvent(OnKeyHoldedEvent { keycode });
            break;
        case GLFW_RELEASE:
            //HandleInputEvent(OnKeyReleasedEvent { keycode });
            s_glfwInputEvent(OnKeyReleasedEvent { keycode });
            break;
        default:
            ENGINE_FATAL("Not defined key action: %d", action);
            break;
        }
    }

    static EMouseButton ConvertToNadorMouseButton(int32_t mouseButton)
    {
        return static_cast<EMouseButton>(mouseButton);
    }

    static int32_t ConvertToNativeMouseButton(EMouseButton mouseButton)
    {
        return static_cast<int32_t>(mouseButton);
    }

    static void Glfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int /*mods*/)
    {
        EMouseButton mouseButton   = ConvertToNadorMouseButton(button);
        glm::vec2    mousePosition = GetCursorPosition(window);

        switch (action)
        {
        case GLFW_PRESS:
            //HandleInputEvent(OnMousePressedEvent { mouseButton, mousePosition });
            s_glfwInputEvent(OnMousePressedEvent { mouseButton, mousePosition });
            break;
        case GLFW_RELEASE:
            //HandleInputEvent(OnMouseReleasedEvent { mouseButton, mousePosition });
            s_glfwInputEvent(OnMouseReleasedEvent { mouseButton, mousePosition });
            break;
        default:
            ENGINE_FATAL("Not defined mouse action action: %d", action);
            break;
        }
    }

    static void Glfw_CharCallback(GLFWwindow* /*window*/, uint32_t codePoint)
    {
        wchar_t txt[] = { '\0', '\0', '\0', '\0' };
        memset(txt, 0, sizeof(txt));
        memcpy(txt, &codePoint, sizeof(codePoint));

        std::wstring ws(txt);

        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;

        std::string text = converter.to_bytes(ws);

        // g_onCharEvent(text);
        //HandleInputEvent(OnCharEvent{ text });
        s_glfwInputEvent(OnCharEvent{ text });
    }

	static void Glfw_WindowCloseCallback(GLFWwindow* /*window*/)
    {
        // fire window close event
        //HandleInputEvent(OnWindowClosedEvent{});
        s_glfwInputEvent(OnWindowClosedEvent{});
    }

    
    GLFWInputController::GLFWInputController(void* nativeApiWindow, InputEventHandler inputHandler)
    : _nativeApiWindow(static_cast<GLFWwindow*>(nativeApiWindow))
	, _inputHandler(std::move(inputHandler))
    , _glfwInputListener([this](InputEvents_t e){ _inputHandler.HandleInputEvent(e); })
    {
        NADOR_ASSERT(_nativeApiWindow);

        s_glfwInputEvent += &_glfwInputListener;

        glfwSetKeyCallback(_nativeApiWindow, Glfw_KeyCallback);
        glfwSetCharCallback(_nativeApiWindow, Glfw_CharCallback);
        glfwSetMouseButtonCallback(_nativeApiWindow, Glfw_MouseButtonCallback);
		glfwSetWindowCloseCallback(_nativeApiWindow, Glfw_WindowCloseCallback);
    }

    
    bool GLFWInputController::IsKeyPressed(EKeyCode keyCode) const
    {
		return _HasKeyState(keyCode, GLFW_PRESS);
    }

    
    bool GLFWInputController::IsKeyReleased(EKeyCode keyCode) const
    {
		return _HasKeyState(keyCode, GLFW_RELEASE);
    }

    
    bool GLFWInputController::IsKeyHolded(EKeyCode keyCode) const
    {
		return _HasKeyState(keyCode, GLFW_REPEAT);
    }

    
    bool GLFWInputController::IsMouseButtonPressed(EMouseButton buttonCode) const
    {
		return _HasMouseState(buttonCode, GLFW_PRESS);
    }

    
    bool GLFWInputController::IsMouseButtonReleased(EMouseButton buttonCode) const
    {
		return _HasMouseState(buttonCode, GLFW_RELEASE);
    }

    
    bool GLFWInputController::IsMouseButtonHolded(EMouseButton buttonCode) const
    {
		return _HasMouseState(buttonCode, GLFW_REPEAT);
    }

    
    glm::vec2 GLFWInputController::GetMousePosition() const
    {
        return GetCursorPosition(_nativeApiWindow);
    }

    
    void GLFWInputController::TickBegin() const
    {
        // Poll for and process events
        glfwPollEvents();
    }

    
	bool GLFWInputController::_HasKeyState(EKeyCode keyCode, int32_t state) const
	{
		if (_inputHandler.IsInputKeyHandledByOthers())
        {
            return false;
        }

        NADOR_ASSERT(_nativeApiWindow);

        return state == glfwGetKey(_nativeApiWindow, ConvertToApiKeyCode(keyCode));
	}

    
	bool GLFWInputController::_HasMouseState(EMouseButton buttonCode, int32_t state) const
	{
		if (_inputHandler.IsInputMouseHandledByOthers())
        {
            return false;
        }

        NADOR_ASSERT(_nativeApiWindow);

        return state == glfwGetMouseButton(_nativeApiWindow, ConvertToNativeMouseButton(buttonCode));
	}

} // namespace nador