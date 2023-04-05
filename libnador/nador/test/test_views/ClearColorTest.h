#ifndef __CLEAR_COLOR_TEST_H__
#define __CLEAR_COLOR_TEST_H__

#include "nador/test/ITest.h"

namespace nador
{
	class IVideo;

	class ClearColorTest : public ITest
	{
	public:
		/*!
		 * ClearColorTest constructor.
		 */
		ClearColorTest(const IVideo* video);

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

		const IVideo* _video;
	};
}

#endif // !__CLEAR_COLOR_TEST_H__