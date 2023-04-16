#ifndef __RENDER_DATA_H__
#define __RENDER_DATA_H__

#include <memory>
#include <vector>

#include "nador/utils/Types.h"
#include "nador/video/IVideo.h"

namespace nador
{
    static constexpr glm::mat4 IDENTITY_MATRIX(1.0f);

    using vertices_list_t       = std::vector<glm::vec4>;
    using texture_coords_list_t = std::vector<glm::vec2>;
    using indices_list_t        = std::vector<uint32_t>;

    struct RenderData
    {
        /*!
         * Sets the vertexes (resets with new elements).
         *
         * \param begin		The begin of the new data.
         * \param size		The size of the new data in bytes.
         */
        void SetVertices(const float_t* begin, size_t size);

        /*!
         * Add vertexes to the current buffer.
         *
         * \param begin		The begin of the new data.
         * \param size		The size of the new data in bytes.
         */
        void AddVertices(const float_t* begin, size_t size);

        /*!
         * Sets the vertexes (resets with new elements).
         *
         * \param matrix		The texture coord matrix.
         */
        void SetVertices(const glm::mat4& matrix);

        glm::vec2 GetBoundingBox() const;

        /*!
         * Add vertexes to the current buffer.
         *
         * \param matrix		The texture coord matrix.
         */
        void AddVertices(const glm::mat4& matrix);

        void SetVertices(const vertices_list_t& list);
        void AddVertices(const vertices_list_t& list);

        /*!
         * Sets the indices (resets with new elements).
         *
         * \param begin		The begin of the new data.
         * \param size		The size of the new data in bytes.
         */
        void SetIndices(const uint32_t* begin, size_t size);

        /*!
         * Add the indices to the container.
         *
         * \param begin		The begin of the new data.
         * \param size		The size of the new data in bytes.
         */
        void AddIndices(const uint32_t* begin, size_t size);

        void SetIndices(const indices_list_t& list);
        void AddIndices(const indices_list_t& list);
        void SetSquareIndices();

        /*!
         * Sets the texture coords (resets with new elements).
         *
         * \param begin		The begin of the new data.
         * \param size		The size of the new data in bytes.
         */
        void SetTextureCoords(const float_t* begin, size_t size);

        /*!
         * Add the texture coords.
         *
         * \param begin		The begin of the new data.
         * \param size		The size of the new data in bytes.
         */
        void AddTextureCoords(const float_t* begin, size_t size);

        /*!
         * Sets the texture coords.
         *
         * \param matrix		The texture coord matrix.
         */
        void SetTextureCoords(const glm::mat4x2& matrix);

        /*!
         * Add texture coords.
         *
         * \param matrix		The texture coord matrix.
         */
        void AddTextureCoords(const glm::mat4x2& matrix);

        void SetTextureCoords(const texture_coords_list_t& list);
        void AddTextureCoords(const texture_coords_list_t& list);

        void Clear();

        bool Empty() const noexcept;

        /*!
         * Sets the container with data.
         *
         * \param container		The container of the new data.
         * \param begin			The begin of the new data.
         * \param size			The size of the new data in bytes.
         */
        template <typename ContainerTy>
        void SetContainer(ContainerTy& container, const void* begin, size_t size)
        {
            container.clear();
            container.resize(size / sizeof(typename ContainerTy::value_type));
            std::memcpy(&container[0], begin, size);
        }

        /*!
         * Add data to the container content.
         *
         * \param container		The container of the data.
         * \param begin			The begin of the new data.
         * \param size			The size of the new data in bytes.
         */
        template <typename ContainerTy>
        void AddContainer(ContainerTy& container, const void* begin, size_t size)
        {
            size_t oldSize = container.size();
            size_t newSize = container.size() + (size / sizeof(typename ContainerTy::value_type));
            container.resize(newSize);
            std::memcpy(&container[oldSize], begin, size);
        }

        vertices_list_t       vertices {};
        texture_coords_list_t texCoords {};
        indices_list_t        indices {};

        EDrawMode drawMode { EDrawMode::E_TRIANGLES };

        static const indices_list_t s_squareIndices;
    };
    CREATE_PTR_TYPES(RenderData);

    glm::vec2 GetBoundingBox(const vertices_list_t& vertices);

    void VerticesMultiMatrix(vertices_list_t& vertices, const glm::mat4& matrix);
    void ConcatIndices(indices_list_t& indicesBase, const indices_list_t& toAdd);

    vertices_list_t operator*(const vertices_list_t& vertices, const glm::mat4& matrix);

    std::string ToString(const glm::vec4& vec);
    std::string ToString(const glm::vec3& vec);
    std::string ToString(const glm::vec2& vec);

    std::string ToString(const vertices_list_t& list);
} // namespace nador

#endif // !__RENDER_DATA_H__
