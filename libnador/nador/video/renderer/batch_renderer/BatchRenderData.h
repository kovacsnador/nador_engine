#ifndef __NADOR_BATCH_RENDER_DATA_H__
#define __NADOR_BATCH_RENDER_DATA_H__

#include "nador/utils/Types.h"
#include "nador/video/renderer/RenderData.h"

namespace nador
{
	struct BatchVertex
	{
		glm::vec4 position;
		glm::vec4 color;
		glm::vec2 textureCoords;
		float_t	  textureId;
		float_t	  isText;
	};

	using BatchVerticesList_t = std::vector<BatchVertex>;

	struct BatchRenderData
	{
		BatchVerticesList_t	vertices;
		indices_list_t	indices;


		size_t Size() const noexcept;
		void Clear();
		void Add(const BatchRenderData& other);
	};

	BatchRenderData CreateBatchRenderData(const RenderData& renderData, glm::vec4 color, float_t textureId, float_t isText);
}

#endif // !__NADOR_BATCH_RENDER_DATA_H__
