#include "BatchRenderData.h"

namespace nador
{
	void BatchRenderData::Clear()
	{
		vertices.clear();
		indices.clear();
	}

	size_t BatchRenderData::Size() const noexcept
	{
		return vertices.size();
	}

	void BatchRenderData::Add(const BatchRenderData& other)
	{
		vertices.insert(vertices.end(), other.vertices.begin(), other.vertices.end());
		ConcatIndices(indices, other.indices);
	}

	BatchRenderData CreateBatchRenderData(const RenderData& renderData, glm::vec4 color, float_t textureId, float_t isText)
	{
		BatchRenderData result;

		for (size_t i = 0; i < renderData.vertices.size(); ++i)
		{
			if (textureId)
			{
				result.vertices.push_back({ renderData.vertices.at(i), color, renderData.texCoords.at(i), textureId, isText });
			}
			else
			{
				result.vertices.push_back({ renderData.vertices.at(i), color, {0, 0}, textureId, isText});
			}
		}
		result.indices = renderData.indices;

		return result;
	}
}