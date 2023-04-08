#ifndef __ATLAS_TEST_H__
#define __ATLAS_TEST_H__

#include "nador/test/ITest.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"

namespace nador
{
	class IAtlasController;

	class AtlasTest : public ITest
	{
	public:
		/*!
		 * AtlasTest constructor.
		 */
		AtlasTest(const IAtlasController* atlasCtrl);

		/*!
		 * Called on render.
		 */
		void OnRender(IRenderer* renderer) override;

		/*!
		 * Called on imgui render.
		 */
		void OnDebugRender(IRenderer* renderer) override;

	private:
		const IAtlasController* _atlasCtrl;

		TextureMaterial _material;
		RenderData		_renderData;
	};
}

#endif // !__ATLAS_TEST_H__