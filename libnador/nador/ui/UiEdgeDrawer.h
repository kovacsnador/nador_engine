#ifndef __UI_EDGE_DRAWER_H__
#define __UI_EDGE_DRAWER_H__

#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"
#include "nador/ui/UiCommon.h"

namespace nador
{
	class UiEdgeDrawer
	{
	public:
		UiEdgeDrawer();
		UiEdgeDrawer(const glm::vec4& uColor);

		void SetColor(const glm::vec4& uColor);

		void Render(IRenderer* renderer, const quadVertices_t& vertices, const glm::vec3& scale);

	private:
		void Init();

		BaseMaterial _material;
		RenderData	 _renderData;
	};
}

#endif // !__UI_EDGE_DRAWER_H__
