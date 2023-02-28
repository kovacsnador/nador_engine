#ifndef __I_WINDOW_H__
#define __I_WINDOW_H__

#include "nador/utils/Types.h"

namespace nador
{
	class IWindow
	{
	public:
		/*!
		 * Virtual destructor.
		 */
		virtual ~IWindow() = default;

		/*!
		 * Creates a window to draw.
		 *
		 * \param width The window width.
		 * \param height The window height.
		 * \param title The window title.
		 */
		virtual void CreateWindow(int32_t width, int32_t height, const char* title, bool vSync) = 0;

		/*!
		 * Creates a full screen window to draw.
		 *
		 * \param title The window title.
		 */
		virtual void CreateFullScreenWindow(const char* title, bool vSync) = 0;

		/*!
		 * The Tick begin function.
		 */
		virtual void TickBegin() = 0;
		
		/*!
		 * The Tick end function.
		 */
		virtual void TickEnd() = 0;

		/*!
		 * Gets the native api window object.
		 *
		 * \return	The api specific window.
		 */
		virtual void* GetNativeApiWindow() = 0;

		virtual void* GetNativeContext() = 0;

		/*!
		 * Shows and hides the debug window.
		 *
		 * \param show	The flag.
		 */
		virtual void ShowDebugWindow(bool show) = 0;

		virtual void InitImGui() = 0;
	};
}

#endif // !__I_WINDOW_H__
