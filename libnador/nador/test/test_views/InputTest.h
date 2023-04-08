#ifndef __INPUT_TEST_H__
#define __INPUT_TEST_H__

#include <string>

#include "nador/test/ITest.h"
#include "nador/common/GlobalEvents.h"

namespace nador
{
	class IInputController;

	class InputTest : public ITest
	{
	public:
		/*!
		 * InputTest constructor.
		 */
		InputTest(const IInputController* inputCtrl);

		/*!
		 * InputTest destructor.
		 */
		~InputTest();

		/*!
		 * Render the imgui content.
		 */
		void OnDebugRender(IRenderer* /*renderer*/) override;

	private:
		const static uint32_t bufferSize = 100;

		onKey_listener_t _keyPressedListener;
		onKey_listener_t _keyHoldedListener;
		onKey_listener_t _keyReleasedListener;

		onMouse_listener_t _mousePressedListener;
		onMouse_listener_t _mouseReleasedListener;

		void _KeyPressedCallback(EKeyCode keyCode);
		void _KeyHoldedCallback(EKeyCode keyCode);
		void _KeyReleasedCallback(EKeyCode keyCode);

		void _MousePressedCallback(EMouseButton mouseButton, const glm::vec2& position);
		void _MouseReleasedCallback(EMouseButton mouseButton, const glm::vec2& position);

		char _additionalMsg[bufferSize];
		char _additionalMsgMouse[bufferSize];

		const IInputController* _inputCtrl;
	};
}

#endif // !__INPUT_TEST_H__