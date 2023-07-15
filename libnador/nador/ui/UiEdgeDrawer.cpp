#include "nador/ui/UiEdgeDrawer.h"
#include "nador/utils/Utils.h"

namespace nador
{
	UiEdgeDrawer::UiEdgeDrawer()
	{
		auto& randEngine = utils::RandomEngine<std::mt19937>();
		std::uniform_real_distribution<> dist(0.2f, 0.8f);
		_material.uColor = { dist(randEngine), dist(randEngine), dist(randEngine), 0.7f };

		Init();
	}

	UiEdgeDrawer::UiEdgeDrawer(const glm::vec4& uColor)
	{
		Init();

		_material.uColor = uColor;
	}

	void UiEdgeDrawer::SetColor(const glm::vec4& uColor)
	{
		_material.uColor = uColor;
	}

	void UiEdgeDrawer::Render(IRenderer* renderer, const quadVertices_t& vertices, const glm::vec3& scale)
	{
		_renderData.SetVertices(vertices);

		glm::mat4 modelMtx = CreateModelMtxWithScale(vertices, scale);

		renderer->Draw(&_material, _renderData, modelMtx);
	}

	void UiEdgeDrawer::Init()
	{
		_renderData.indices = { 0, 1, 1, 2, 2, 3, 3, 0 };
		_renderData.drawMode = EDrawMode::E_LINES;
	}
}