#ifndef __NADOR_GLFW_INPUT_CONTROLLER_H__
#define __NADOR_GLFW_INPUT_CONTROLLER_H__

#include "nador/utils/event/Event.h"
#include "nador/system/IInputController.h"
#include "nador/system/input/input_events/InputEvents.h"

struct GLFWwindow;

namespace nador
{
    class GLFWInputController : public IInputController
    {
    public:
        /*!
         * GLFWInputController constructor.
         *
         * \param nativeApiWindow      The native window.
         */
        GLFWInputController(void* nativeApiWindow, InputEventHandler inputHandler);

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
        bool _HasKeyState(EKeyCode keyCode, int32_t state) const;
        bool _HasMouseState(EMouseButton buttonCode, int32_t state) const;

        GLFWwindow*    _nativeApiWindow;
        InputEventHandler _inputHandler;

        EventListener<InputEvents_t> _glfwInputListener;
    };
} // namespace nador

#endif // !__NADOR_GLFW_INPUT_CONTROLLER_H__