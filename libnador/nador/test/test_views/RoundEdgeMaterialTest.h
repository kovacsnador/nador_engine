#ifndef __ROUND_EDGE_MATERIAL_TEST_H__
#define __ROUND_EDGE_MATERIAL_TEST_H__

#include "nador/video/renderer/IRenderer.h"
#include "nador/test/ITest.h"

namespace nador
{
	class RoundEdgeMaterialTest : public ITest
	{
	public:
		RoundEdgeMaterialTest()
		{
			_renderData.vertices = {
				{0.0f,	0.0f,	0.0f, 1.0f},
				{200.0f, 0.0f,	0.0f, 1.0f },
				{200.0f, 100.0f, 1.0f, 1.0f},
				{0.0f,	100.0f, 0.0f, 1.0f},
			};

			_renderData.indices = {
				0, 1, 2,
				2, 3, 0,
			};

			_renderData.texCoords = {
				{0.0f, 0.0f},
				{1.0f, 0.0f},
				{1.0f, 1.0f},
				{0.0f, 1.0f},
			};

			_material.uColor = { 1.0f, 1.0f, 0.0f, 1.0f };
			_material.uDimensions = { 200, 100 };
			_material.uRadius = 10;
		}

		void OnRender(IRenderer* renderer) override
		{
			glm::vec3 myRotationAxis(0, 0, 1);
			glm::mat4 rotationMatrix = glm::rotate(25.0f, myRotationAxis);
			
			glm::mat4 translate = glm::translate(glm::vec3(100.0f, 100.0f, 0.0f ));

			glm::mat4 modelMatrix = translate * rotationMatrix;

			renderer->Draw(&_material, _renderData, modelMatrix);
		}

	private:
		RoundEdgeMaterial _material;
		RenderData		  _renderData;
	};
}

#endif // !__ROUND_EDGE_MATERIAL_TEST_H__