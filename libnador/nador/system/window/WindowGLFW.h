#ifndef __WINDOW_GLFW_H__
#define __WINDOW_GLFW_H__

#include "nador/system/IWindow.h"

struct GLFWwindow;

namespace nador
{
	class WindowGLFW : public IWindow
	{
	public:
		/*!
		 * Virtual destructor.
		 */
		virtual ~WindowGLFW();

		/*!
		 * Creates an opengl window to draw.
		 *
		 * \param width The window width.
		 * \param height The window height.
		 * \param title The window title.
		 * \param vSync The vSync flag.
		 */
		void CreateWindow(int32_t width, int32_t height, const char* title, bool vSync) override;

		/*!
		 * Creates a full screen window to draw.
		 *
		 * \param title The window title.
		 * \param vSync The vSync flag.
		 */
		void CreateFullScreenWindow(const char* title, bool vSync) override;

		/*!
		 * The Tick begin function.
		 */
		void TickBegin() override;

		/*!
		 * The Tick end function.
		 */
		void TickEnd() override;

		/*!
		 * Gets the api window object.
		 *
		 * \return	The api specific window.
		 */
		void* GetNativeApiWindow() override;

		void* GetNativeContext() override;

		/*!
		 * Shows and hides the debug window.
		 *
		 * \param show	The flag.
		 */
		void ShowDebugWindow(bool show) override;

		/*!
		 * Initializes an imGui context.
		 */
		void InitImGui() override;

	private:

		/*!
		 * Start new frame with imGui.
		 */
		void _NewFrameImGui();

		/*!
		 * Ends a frame with imGui.
		 */
		void _EndFrameImGui();


		GLFWwindow* _window {nullptr};
		bool		_showDebugWindow {false};
	};
}

#endif // !__WINDOW_GLFW_H__
