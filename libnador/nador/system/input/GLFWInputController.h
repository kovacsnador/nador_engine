#ifndef __WINDOWS_INPUT_CONTROLLER_H__
#define __WINDOWS_INPUT_CONTROLLER_H__

#include "nador/system/IInputController.h"

struct GLFWwindow;

namespace nador
{
	// TODO: rename it to InputControllerGLFW
	
	class GLFWInputController : public IInputController
	{
	public:
		/*!
		 * GLFWInputController constructor.
		 *
		 * \param nativeApiWindow      The native window.
		 */
		GLFWInputController(void* nativeApiWindow);

		/*!
		 * Detects if key was pressed.
		 *
		 * \param keyCode      The code of the key.
		 *
		 * \return	True if key was pressed, false otherwise.
		 */
		bool IsKeyPressed(EKeyCode keyCode) const override;

		/*!
		 * Detects if key was released.
		 *
		 * \param keyCode      The code of the key.
		 *
		 * \return	True if key was released, false otherwise.
		 */
		bool IsKeyReleased(EKeyCode keyCode) const override;

		/*!
		 * Detects if key was holded.
		 *
		 * \param keyCode      The code of the key.
		 *
		 * \return	True if key was holded, false otherwise.
		 */
		bool IsKeyHolded(EKeyCode keyCode) const override;

		/*!
		 * Detects if mouse button was pressed.
		 *
		 * \param buttonCode      The code of the mouse button.
		 *
		 * \return	True if mouse button was pressed, false otherwise.
		 */
		bool IsMouseButtonPressed(EMouseButton buttonCode) const override;

		/*!
		 * Detects if mouse button was released.
		 *
		 * \param buttonCode      The code of the mouse button.
		 *
		 * \return	True if mouse button was released, false otherwise.
		 */
		bool IsMouseButtonReleased(EMouseButton buttonCode) const override;

		/*!
		 * Detects if mouse button was holded.
		 *
		 * \param buttonCode      The code of the mouse button.
		 *
		 * \return	True if mouse button was holded, false otherwise.
		 */
		bool IsMouseButtonHolded(EMouseButton buttonCode) const override;

		/*!
		 * Gets the mouse position on the screen.
		 *
		 * \param buttonCode      The code of the mouse button.
		 *
		 * \return	The mouse position.
		 */
		glm::vec2 GetMousePosition() const override;

		void TickBegin() override;

	private:
		GLFWwindow* _nativeApiWindow;
	};
}

#endif // !__WINDOWS_INPUT_CONTROLLER_H__