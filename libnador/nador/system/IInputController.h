#ifndef __IINPUT_CONTROLLER__
#define __IINPUT_CONTROLLER__

#include "nador/utils/Types.h"
#include "nador/common/KeyCodes.h"
#include "nador/common/Mouse.h"

namespace nador
{
	class IInputController
	{
	public:
		/*!
		 * IInputController destructor.
		 */
		virtual ~IInputController() = default;

		/*!
		 * Detects if key was pressed.
		 *
		 * \param keyCode      The code of the key.
		 * 
		 * \return	True if key was pressed, false otherwise.
		 */
		virtual bool IsKeyPressed(EKeyCode keyCode) const = 0;

		/*!
		 * Detects if key was released.
		 *
		 * \param keyCode      The code of the key.
		 *
		 * \return	True if key was released, false otherwise.
		 */
		virtual bool IsKeyReleased(EKeyCode keyCode) const = 0;

		/*!
		 * Detects if key was holded.
		 *
		 * \param keyCode      The code of the key.
		 *
		 * \return	True if key was holded, false otherwise.
		 */
		virtual bool IsKeyHolded(EKeyCode keyCode) const = 0;
		
		/*!
		 * Detects if mouse button was pressed.
		 *
		 * \param buttonCode      The code of the mouse button.
		 * 
		 * \return	True if mouse button was pressed, false otherwise.
		 */
		virtual bool IsMouseButtonPressed(EMouseButton buttonCode) const = 0;

		/*!
		 * Detects if mouse button was released.
		 *
		 * \param buttonCode      The code of the mouse button.
		 *
		 * \return	True if mouse button was released, false otherwise.
		 */
		virtual bool IsMouseButtonReleased(EMouseButton buttonCode) const = 0;

		/*!
		 * Detects if mouse button was holded.
		 *
		 * \param buttonCode      The code of the mouse button.
		 *
		 * \return	True if mouse button was holded, false otherwise.
		 */
		virtual bool IsMouseButtonHolded(EMouseButton buttonCode) const = 0;
		
		/*!
		 * Gets the mouse position on the screen.
		 *
		 * \param buttonCode      The code of the mouse button.
		 * 
		 * \return	The mouse position.
		 */
		virtual glm::vec2 GetMousePosition() const = 0;

		/*!
		 * On tick begin fundtion to pull all input events.
		 */
		virtual void TickBegin() = 0;
	};
}

#endif //!__IINPUT_CONTROLLER__
