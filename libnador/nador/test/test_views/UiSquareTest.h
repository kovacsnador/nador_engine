#ifndef __UI_SQUARE_TEST_H__
#define __UI_SQUARE_TEST_H__

#include "nador/test/ITest.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"

namespace nador
{
	struct TestImage
	{
		TestImage();

		glm::mat4 Update(UiAlignment aligner, const glm::mat4& parent);

		glm::ivec2  pos;
		glm::ivec2  size;

		TextureMaterial material;
		RenderData		renderData;
	};

	class UiSquareTest : public ITest
	{
	public:
		UiSquareTest(const IVideo* video);

		void OnRender(IRenderer* renderer) override;
		void OnDebugRender() override;

	private:
		void _Update();

		TestImage _parent;
		TestImage _child;

		int32_t _parentSelectedHorizontal{ 0 };
		int32_t _parentSelectedVertical{ 0 };

		int32_t _childSelectedHorizontal{ 0 };
		int32_t _childSelectedVertical{ 0 };
	};
}

#endif // !__UI_SQUARE_TEST_H__
