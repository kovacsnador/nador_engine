#ifndef __SIMPLE_LINE_TEST_H__
#define __SIMPLE_LINE_TEST_H__

#include "nador/App.h"
#include "nador/test/ITest.h"
#include "nador/video/renderer/RenderData.h"

namespace nador
{
	class SimpleLineTest : public ITest
	{
	public:
		SimpleLineTest(const IVideo* video)
		: ITest(video)
		{
			_renderData.vertices = {
				{.1f,	0.0f,	0.0f, 1.0f},
				{100.0f, 0.0f,	0.0f, 1.0f },
				{100.0f, 100.0f, 0.0f, 1.0f},
				{.1f,	100.0f, 0.0f, 1.0f},
			};

			_renderData.indices = {
				0, 1, 1, 2, 2, 3, 3, 0
			};

			_renderData.drawMode = EDrawMode::E_LINES;

			_material.uColor = { 1.0f, 1.0f, 0.0f, .5f };
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

#endif // !__SIMPLE_LINE_TEST_H__

