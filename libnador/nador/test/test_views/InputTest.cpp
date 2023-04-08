#include "imgui.h"

#include "nador/test/test_views/InputTest.h"
#include "nador/common/GlobalEvents.h"
#include "nador/utils/Utils.h"
#include "nador/system/IInputController.h"

namespace nador
{
	const uint32_t InputTest::bufferSize;

	InputTest::InputTest(const IInputController* inputCtrl)
	: _inputCtrl(inputCtrl)
	{
		memset(_additionalMsg, 0, bufferSize * sizeof(*_additionalMsg));
		memset(_additionalMsgMouse, 0, bufferSize * sizeof(*_additionalMsgMouse));

		g_onKeyPressedEvent += &_keyPressedListener;
		g_onKeyHoldedEvent += &_keyHoldedListener;
		g_onKeyReleasedEvent += &_keyReleasedListener;

		_keyPressedListener.SetCallback(std::bind(&InputTest::_KeyPressedCallback, this, arg::_1));
		_keyHoldedListener.SetCallback(std::bind(&InputTest::_KeyHoldedCallback, this, arg::_1));
		_keyReleasedListener.SetCallback(std::bind(&InputTest::_KeyReleasedCallback, this, arg::_1));

		g_onMousePressedEvent += &_mousePressedListener;
		g_onMouseReleasedEvent += &_mouseReleasedListener;

		_mousePressedListener.SetCallback(std::bind(&InputTest::_MousePressedCallback, this, arg::_1, arg::_2));
		_mouseReleasedListener.SetCallback(std::bind(&InputTest::_MouseReleasedCallback, this, arg::_1, arg::_2));
	}

	InputTest::~InputTest()
	{
		g_onKeyPressedEvent -= &_keyPressedListener;
		g_onKeyHoldedEvent -= &_keyHoldedListener;
		g_onKeyReleasedEvent -= &_keyReleasedListener;
	}

	void InputTest::OnDebugRender(IRenderer* /*renderer*/)
	{
		glm::vec2 mousePosition = _inputCtrl->GetMousePosition();

		ImGui::Text("Mouse position: x:%f, y:%f", mousePosition.x, mousePosition.y);
		ImGui::Text(_additionalMsg);
		ImGui::Text(_additionalMsgMouse);
	}

	void InputTest::_KeyPressedCallback(EKeyCode keyCode)
	{
		snprintf(_additionalMsg, bufferSize, "Key was pressed: %d", utils::ToIntegral(keyCode));
	}

	void InputTest::_KeyHoldedCallback(EKeyCode keyCode)
	{
		snprintf(_additionalMsg, bufferSize, "Key was holded: %d", utils::ToIntegral(keyCode));
	}

	void InputTest::_KeyReleasedCallback(EKeyCode keyCode)
	{
		snprintf(_additionalMsg, bufferSize, "Key was released: %d", utils::ToIntegral(keyCode));
	}

	void InputTest::_MousePressedCallback(EMouseButton mouseButton, const glm::vec2& position)
	{
		snprintf(_additionalMsgMouse, bufferSize, "Mousebutton %d was pressed in position x:%f y:%f ", utils::ToIntegral(mouseButton), position.x, position.y);
	}
	
	void InputTest::_MouseReleasedCallback(EMouseButton mouseButton, const glm::vec2& position)
	{
		snprintf(_additionalMsgMouse, bufferSize, "Mousebutton %d was released in position x:%f y:%f ", utils::ToIntegral(mouseButton), position.x, position.y);
	}
}