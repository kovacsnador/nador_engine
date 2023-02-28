#include <string>
#include <codecvt>
#include <locale>

#include <GLFW/glfw3.h>

#include "nador/system/input/GLFWInputController.h"
#include "nador/log/Log.h"

#include "nador/common/GlobalEvents.h"

namespace nador
{
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
		EKeyCode keyCode = ConvertToNadorKeyCode(key);

		switch(action)
		{
		case GLFW_PRESS:
			g_onKeyPressedEvent(keyCode);
			break;
		case GLFW_REPEAT:
			g_onKeyHoldedEvent(keyCode);
			break;
		case GLFW_RELEASE:
			g_onKeyReleasedEvent(keyCode);
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

	static void GLFW_MouseButtonCallback(GLFWwindow* window, int button, int action, int /*mods*/)
	{
		EMouseButton mouseButton = ConvertToNadorMouseButton(button);
		glm::vec2 mousePosition = GetCursorPosition(window);

		switch(action)
		{
		case GLFW_PRESS:
			g_onMousePressedEvent(mouseButton, mousePosition);
			break;
		case GLFW_RELEASE:
			g_onMouseReleasedEvent(mouseButton, mousePosition);
			break;
		default:
			ENGINE_FATAL("Not defined mouse action action: %d", action);
			break;
		}
	}

	static void GLFW_CharCallback(GLFWwindow* /*window*/, uint32_t codePoint)
	{
		wchar_t txt[] = {'\0', '\0', '\0', '\0'};
		memset(txt, 0, sizeof(txt));
		memcpy(txt, &codePoint, sizeof(codePoint));

		std::wstring ws(txt);

		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;

		std::string text = converter.to_bytes(ws);

		g_onCharEvent(text);
	}

	GLFWInputController::GLFWInputController(void* nativeApiWindow)
	: _nativeApiWindow(static_cast<GLFWwindow*>(nativeApiWindow))
	{
		NADOR_ASSERT(_nativeApiWindow);

		glfwSetKeyCallback(_nativeApiWindow, Glfw_KeyCallback);
		glfwSetCharCallback(_nativeApiWindow, GLFW_CharCallback);
		glfwSetMouseButtonCallback(_nativeApiWindow, GLFW_MouseButtonCallback);
	}

	bool GLFWInputController::IsKeyPressed(EKeyCode keyCode) const
	{
		NADOR_ASSERT(_nativeApiWindow);

		int32_t state = glfwGetKey(_nativeApiWindow, ConvertToApiKeyCode(keyCode));
		return state == GLFW_PRESS;
	}

	bool GLFWInputController::IsKeyReleased(EKeyCode keyCode) const
	{
		NADOR_ASSERT(_nativeApiWindow);

		int32_t state = glfwGetKey(_nativeApiWindow, ConvertToApiKeyCode(keyCode));
		return state == GLFW_RELEASE;
	}

	bool GLFWInputController::IsKeyHolded(EKeyCode keyCode) const
	{
		NADOR_ASSERT(_nativeApiWindow);

		int32_t state = glfwGetKey(_nativeApiWindow, ConvertToApiKeyCode(keyCode));
		return state == GLFW_REPEAT;
	}

	bool GLFWInputController::IsMouseButtonPressed(EMouseButton buttonCode) const
	{
		NADOR_ASSERT(_nativeApiWindow);

		int32_t state = glfwGetMouseButton(_nativeApiWindow, ConvertToNativeMouseButton(buttonCode));
		return state == GLFW_PRESS;
	}

	bool GLFWInputController::IsMouseButtonReleased(EMouseButton buttonCode) const
	{
		NADOR_ASSERT(_nativeApiWindow);

		int32_t state = glfwGetMouseButton(_nativeApiWindow, ConvertToNativeMouseButton(buttonCode));
		return state == GLFW_RELEASE;
	}

	bool GLFWInputController::IsMouseButtonHolded(EMouseButton buttonCode) const
	{
		NADOR_ASSERT(_nativeApiWindow);

		int32_t state = glfwGetMouseButton(_nativeApiWindow, ConvertToNativeMouseButton(buttonCode));
		return state == GLFW_REPEAT;
	}

	glm::vec2 GLFWInputController::GetMousePosition() const
	{
		return GetCursorPosition(_nativeApiWindow);
	}

    void GLFWInputController::TickBegin()
	{
		// Poll for and process events
		glfwPollEvents();
	}
}