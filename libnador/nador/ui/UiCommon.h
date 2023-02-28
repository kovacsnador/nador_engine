#ifndef __UI_COMMON_H__
#define __UI_COMMON_H__

#include <array>

#include "nador/video/renderer/Renderer.h"
#include "nador/video/atlas/Atlas.h"

#include "EImageName.h"

namespace nador
{
	using quadVertices_t = glm::mat4;

	enum class EUiLayer
	{
		BACKGROUND = 0,
		OVERLAY,
		DIALOG,
	};

	enum class EHorizontalAlignment : uint32_t
	{
		LEFT = 0,
		RIGHT,
		CENTER, 
		STRETCH,
	};

	enum class EVerticalAlignment : uint32_t
	{
		TOP = 0,
		BOTTOM,
		CENTER,
		STRETCH,
	};

	struct UiAlignment
	{
		EHorizontalAlignment eHorizontAlign{ EHorizontalAlignment::LEFT };
		EVerticalAlignment	 eVerticAlign{ EVerticalAlignment::TOP };

		quadVertices_t GenerateVertices(const glm::ivec2& pos, const glm::ivec2& size, const quadVertices_t& parent) const;
	};

	bool IsPointOverOnVertices(const glm::vec2& point, const quadVertices_t& vertices);

	glm::vec3 GetAverage(const quadVertices_t& vertices);

	ImageData GetImageData(video::EImageName name);

	glm::vec4 GetPivot(const quadVertices_t& vertices);

	glm::mat4 CreateModelMtxWithScale(const quadVertices_t& vertices, const glm::vec3& scale);
}

#endif // !__UI_COMMON_H__
