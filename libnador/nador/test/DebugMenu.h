#ifndef __DEBUG_MENU_H__
#define __DEBUG_MENU_H__

#include "nador/test/ITest.h"

namespace nador
{
	class DebugMenu : public ITest 
	{
	public:
		/*!
		 * DebugMenu destructor.
		 */
		virtual ~DebugMenu() = default;


		/*!
		 * Render the imgui content.
		 */
		void OnDebugRender() override;
	};
}

#endif // !__DEBUG_MENU_H__

