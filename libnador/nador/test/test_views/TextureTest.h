#ifndef __TEXTURE_TEST_H__
#define __TEXTURE_TEST_H__

#include "nador/test/ITest.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"


namespace nador
{
	class IVideo;
	class IFileController;

	class TextureTest : public ITest
	{
	public:
		/*!
		 * TextureTest constructor.
		 */
		TextureTest(const IVideo* video, const IFileController* fileCtrl);

		/*!
		 * Called on render.
		 */
		void OnRender(IRenderer* renderer) override;

		/*!
		 * Called on imgui render.
		 */
		void OnDebugRender() override;

	private:
		const IVideo* 		   _video;
		const IFileController* _fileCtrl;

		TextureMaterial _material;
		RenderData		_renderData;
		RenderData		_renderData2;
		RenderData		_renderData3;
		glm::vec3		_translation;
	};
}

#endif // !__TEXTURE_TEST_H__

