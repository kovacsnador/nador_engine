#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include <memory>

#include "nador/video/IVideo.h"

namespace nador
{
	class VertexBuffer
	{
	public:
		enum class EBufferType
		{
			STATIC,
			DYNAMIC
		};


		/*!
		 * Vertex buffer constructor.
		 *
		 * \param data	The vertex data to upload.
		 * \param size	Specifies the size in bytes of the buffer object's new data store.
		 * \param type	The buffer type.
		 */
		VertexBuffer(const void* data, size_t size, EBufferType type = EBufferType::STATIC);

		/*!
		 * Vertex buffer destructor.
		 */
		~VertexBuffer();

		/*!
		 * Bind the vertex buffer.
		 */
		void Bind() const;

		void BufferSubData(uint32_t location, size_t size, const void* data) const;

		/*!
		 * Unbind the vertex buffer.
		 */
		void Unbind() const;

	private:
		uint32_t		_id;
		const IVideo*	_video;
	};

	CREATE_PTR_TYPES(VertexBuffer);
}

#endif // !__VERTEX_BUFFER_H__

