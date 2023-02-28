#ifndef __ATLAS_TEST_H__
#define __ATLAS_TEST_H__

#include "nador/test/ITest.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"

namespace nador
{
	class AtlasTest : public ITest
	{
	public:
		/*!
		 * AtlasTest constructor.
		 */
		AtlasTest();

		/*!
		 * Called on render.
		 */
		void OnRender(IRenderer* renderer) override;

		/*!
		 * Called on imgui render.
		 */
		void OnDebugRender() override;

	private:
		TextureMaterial _material;
		RenderData		_renderData;
	};
}

#endif // !__ATLAS_TEST_H__