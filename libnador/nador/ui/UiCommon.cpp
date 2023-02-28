#include "nador/ui/UiCommon.h"
#include "nador/App.h"

namespace nador
{
	quadVertices_t UiAlignment::GenerateVertices(const glm::ivec2& pos, const glm::ivec2& size, const quadVertices_t& parent) const
	{
		quadVertices_t vertices = parent;

		// Vertical alignemnts
		if (EVerticalAlignment::BOTTOM == eVerticAlign)
		{
			vertices[0].y += pos.y;
			vertices[1].y += pos.y;
			vertices[2].y = vertices[0].y + size.y;
			vertices[3].y = vertices[0].y + size.y;
		}
		else if (EVerticalAlignment::TOP == eVerticAlign)
		{
			vertices[2].y -= pos.y;
			vertices[3].y -= pos.y;
			vertices[0].y = vertices[2].y - size.y;
			vertices[1].y = vertices[2].y - size.y;
		}
		else if (EVerticalAlignment::CENTER == eVerticAlign)
		{
			float_t parentHeight = vertices[2].y - vertices[0].y;
			float_t halfParentHeight = parentHeight / 2;

			int32_t halfHeight = size.y / 2;

			float_t min = vertices[0].y + halfParentHeight - halfHeight;
			float_t max = vertices[0].y + halfParentHeight + halfHeight;

			vertices[0].y = min;
			vertices[1].y = min;
			vertices[2].y = max;
			vertices[3].y = max;
		}
		else if (EVerticalAlignment::STRETCH == eVerticAlign)
		{
			// do nothing 
		}
		else
		{
			ENGINE_FATAL("Vertical alignemnt %d not implemented!", eVerticAlign);
		}

		// Horizontal alignemnts
		if (EHorizontalAlignment::LEFT == eHorizontAlign)
		{
			vertices[0].x += pos.x;
			vertices[1].x = vertices[0].x + size.x;
			vertices[2].x = vertices[0].x + size.x;
			vertices[3].x += pos.x;
		}
		else if (EHorizontalAlignment::RIGHT == eHorizontAlign)
		{
			vertices[1].x -= pos.x;
			vertices[2].x -= pos.x;
			vertices[3].x = vertices[1].x - size.x;
			vertices[0].x = vertices[1].x - size.x;
		}
		else if (EHorizontalAlignment::CENTER == eHorizontAlign)
		{
			float_t parentWidth = vertices[2].x - vertices[0].x;
			float_t halfParentWidth = parentWidth / 2;

			int32_t halfWidth = size.x / 2;

			float_t min = vertices[0].x + halfParentWidth - halfWidth;
			float_t max = vertices[0].x + halfParentWidth + halfWidth;

			vertices[0].x = min;
			vertices[1].x = max;
			vertices[2].x = max;
			vertices[3].x = min;
		}
		else if (EHorizontalAlignment::STRETCH == eHorizontAlign)
		{
			// do nothing 
		}
		else
		{
			ENGINE_FATAL("Horizontal alignemnt %d not implemented!", eHorizontAlign);
		}

		return vertices;
	}

	ImageData GetImageData(video::EImageName name)
	{
		const IAtlasController* atlasCtrl = IApp::Get()->GetAtlasController();

		NADOR_ASSERT(atlasCtrl);

		return atlasCtrl->GetImage(name);
	}

	glm::vec4 GetPivot(const quadVertices_t& vertices)
	{
		return (vertices[0] + vertices[1] + vertices[2] + vertices[3]) * 0.25f;
	}

	bool IsPointOverOnVertices(const glm::vec2& point, const quadVertices_t& vertices)
	{
		return (point.y >= vertices[0][1] &&
				point.y <= vertices[2][1] &&
				point.x >= vertices[0][0] &&
				point.x <= vertices[1][0]);
	}

	glm::vec3 GetAverage(const quadVertices_t& vertices)
	{
		glm::vec3 average;

		average.x = (vertices[0][0] + vertices[1][0] + vertices[2][0] + vertices[3][0]) / 4;
		average.y = (vertices[0][1] + vertices[1][1] + vertices[2][1] + vertices[3][1]) / 4;
		average.z = (vertices[0][2] + vertices[1][2] + vertices[2][2] + vertices[3][2]) / 4;

		return average;
	}

	glm::mat4 CreateModelMtxWithScale(const quadVertices_t& vertices, const glm::vec3& scale)
	{
		glm::vec3 pivot = GetPivot(vertices);
		pivot.z = 0.0f;

		glm::mat4 translateToOrigo = glm::translate(glm::mat4(1.0f), -pivot);		// translate to origo (0, 0)
		glm::mat4 scaleMtx = glm::scale(scale);
		glm::mat4 translateToPosition = glm::translate(glm::mat4(1.0f), pivot);		// translate from origo to position back

		return translateToPosition * scaleMtx * translateToOrigo;		// !!! BEWARE !!! This lines actually performs the translateToOrigo FIRST, and THEN the scaleMtx, and THEN the translateToPosition.
	}
}