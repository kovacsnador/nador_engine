#ifndef __BASE_MATERIAL_TEST_H__
#define __BASE_MATERIAL_TEST_H__

#include "nador/video/renderer/IRenderer.h"
#include "nador/test/ITest.h"
#include "nador/video/renderer/RenderData.h"

namespace nador
{
	class BaseMaterialTest : public ITest
	{
	public:
		BaseMaterialTest()
		{
			_renderData.vertices = {
				{0.0f,	0.0f,	0.0f, 1.0f},
				{100.0f, 0.0f,	0.0f, 1.0f },
				{100.0f, 100.0f, 1.0f, 1.0f},
				{0.0f,	100.0f, 0.0f, 1.0f },
			};

			_renderData.indices = {
				0, 1, 2,
				2, 3, 0,
			};

			_material.uColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		}

		void OnRender(IRenderer* renderer) override
		{
			glm::mat4 modelMatrix(1.0f);
			renderer->Draw(&_material, _renderData, &modelMatrix);
		}

	private:
		BaseMaterial	_material;
		RenderData		_renderData;
	};
}

#endif // !__BASE_MATERIAL_TEST_H__
