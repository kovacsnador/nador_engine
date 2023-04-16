#include <algorithm>

#include "nador/video/renderer/RenderData.h"
#include "nador/utils/Utils.h"

namespace nador
{
	const indices_list_t RenderData::s_squareIndices = { 0, 1, 2, 2, 3, 0, };

	void RenderData::SetVertices(const float_t* begin, size_t size)
	{
		SetContainer(vertices, begin, size);
	}

	void RenderData::AddVertices(const float_t* begin, size_t size)
	{
		AddContainer(vertices, begin, size);
	}

	void RenderData::SetVertices(const glm::mat4& matrix)
	{
		SetVertices(&matrix[0][0], (size_t)sizeof(matrix));
	}

	glm::vec2 RenderData::GetBoundingBox() const
	{
		return nador::GetBoundingBox(vertices);
	}

	void RenderData::AddVertices(const glm::mat4& matrix)
	{
		AddVertices(&matrix[0][0], (size_t)sizeof(matrix));
	}

	void RenderData::SetVertices(const vertices_list_t& list)
	{
		vertices = list;
	}

	void RenderData::AddVertices(const vertices_list_t& list)
	{
		vertices.insert(vertices.end(), list.begin(), list.end());
	}

	void RenderData::SetIndices(const uint32_t* begin, size_t size)
	{
		SetContainer(indices, begin, size);
	}

	void RenderData::AddIndices(const uint32_t* begin, size_t size)
	{
		AddContainer(indices, begin, size);
	}

	void RenderData::SetIndices(const indices_list_t& list)
	{
		indices = list;
	}

	void RenderData::AddIndices(const indices_list_t& list)
	{
		indices.insert(indices.end(), list.begin(), list.end());
	}

	void RenderData::SetSquareIndices()
	{
		indices = s_squareIndices;
	}

	void RenderData::SetTextureCoords(const float_t* begin, size_t size)
	{
		SetContainer(texCoords, begin, size);
	}

	void RenderData::AddTextureCoords(const float_t* begin, size_t size)
	{
		AddContainer(texCoords, begin, size);
	}

	void RenderData::SetTextureCoords(const glm::mat4x2& matrix)
	{
		SetTextureCoords(&matrix[0][0], (size_t)sizeof(matrix));
	}

	void RenderData::AddTextureCoords(const glm::mat4x2& matrix)
	{
		AddTextureCoords(&matrix[0][0], (size_t)sizeof(matrix));
	}

	void RenderData::SetTextureCoords(const texture_coords_list_t& list)
	{
		texCoords = list;
	}

	void RenderData::AddTextureCoords(const texture_coords_list_t& list)
	{
		texCoords.insert(texCoords.end(), list.begin(), list.end());
	}

	void RenderData::Clear()
	{
		vertices.clear();
		indices.clear();
		texCoords.clear();
	}

	bool RenderData::Empty() const noexcept
	{
		return vertices.empty() && indices.empty() && texCoords.empty();
	}

	void ConcatIndices(indices_list_t& indicesBase, const indices_list_t& toAdd)
	{
		if (indicesBase.empty())
		{
			indicesBase = toAdd;
		}
		else
		{
			uint32_t max = *std::max_element(indicesBase.begin(), indicesBase.end()) + 1;
			std::for_each(toAdd.begin(), toAdd.end(),
				[&indicesBase, max](const uint32_t& it) {
					indicesBase.push_back(it + max);
				});
		}
	}

	void VerticesMultiMatrix(vertices_list_t& vertices, const glm::mat4& matrix)
	{
		for (auto& it : vertices)
		{
			it = matrix * it;
		}
	}

	vertices_list_t operator*(const vertices_list_t& vertices, const glm::mat4& matrix)
	{
		vertices_list_t result(vertices.size());

		for (const auto& it : vertices)
		{
			result.push_back(it * matrix);
		}

		return result;
	}

	glm::vec2 GetBoundingBox(const vertices_list_t& vertices)
	{
		glm::vec2 bb{ 0, 0 };

		if (vertices.empty() == false)
		{
			auto xMinMax = std::minmax_element(vertices.begin(), vertices.end(), [](const glm::vec4& v1, const glm::vec4& v2) { return v1.x < v2.x; });
			auto yMinMax = std::minmax_element(vertices.begin(), vertices.end(), [](const glm::vec4& v1, const glm::vec4& v2) { return v1.y < v2.y; });

			bb.x = xMinMax.second->x - xMinMax.first->x;
			bb.y = yMinMax.second->y - yMinMax.first->y;

		}
		return bb;
	}

	std::string ToString(const glm::vec4& vec)
	{
		constexpr size_t BUFF_SIZE = 256;
		char buff[BUFF_SIZE];

		snprintf(buff, BUFF_SIZE, "%f, %f, %f, %f", vec[0], vec[1], vec[2], vec[3]);

		return buff;
	}

	std::string ToString(const glm::vec3& vec)
	{
		constexpr size_t BUFF_SIZE = 256;
		char buff[BUFF_SIZE];

		snprintf(buff, BUFF_SIZE, "%f, %f, %f", vec[0], vec[1], vec[2]);

		return buff;
	}

	std::string ToString(const glm::vec2& vec)
	{
		constexpr size_t BUFF_SIZE = 256;
		char buff[BUFF_SIZE];

		snprintf(buff, BUFF_SIZE, "%f, %f", vec[0], vec[1]);

		return buff;
	}

	std::string ToString(const vertices_list_t& list)
	{
		std::string result;

		for (auto& it : list)
		{
			result += '\n';
			result += ToString(it);
		}

		return result;
	}
}