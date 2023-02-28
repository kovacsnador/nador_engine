#ifndef __TEXTURE_TEST_H__
#define __TEXTURE_TEST_H__

#include "nador/test/ITest.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"


namespace nador
{
	class TextureTest : public ITest
	{
	public:
		/*!
		 * TextureTest constructor.
		 */
		TextureTest();

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
		RenderData		_renderData2;
		RenderData		_renderData3;
		glm::vec3		_translation;
	};
}

#endif // !__TEXTURE_TEST_H__

