#ifndef __CLEAR_COLOR_TEST_H__
#define __CLEAR_COLOR_TEST_H__

#include "nador/test/ITest.h"

namespace nador
{
	class ClearColorTest : public ITest
	{
	public:
		/*!
		 * ClearColorTest constructor.
		 */
		ClearColorTest();

		/*!
		 * Called on render.
		 */
		void OnRender(IRenderer*) override;

		/*!
		 * Render the imgui content.
		 */
		void OnDebugRender() override;

	private:
		static float_t _clearColor[4];
	};
}

#endif // !__CLEAR_COLOR_TEST_H__