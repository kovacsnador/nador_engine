#include "nador/video/buffers/VertexBuffer.h"
#include "nador/log/ILog.h"

namespace nador
{
	VertexBuffer::VertexBuffer(const IVideo* video, const void* data, size_t size, EBufferType type)
	: _id(0)
	, _video(video)
	{
		NADOR_ASSERT(_video);

		if(_video)
		{
			if (type == EBufferType::STATIC)
			{
				NADOR_ASSERT(data);
				_id = _video->UploadVertexBuffer(data, size);
			}
			else if (type == EBufferType::DYNAMIC)
			{
				_id = _video->UploadVertexBufferDynamic(data, size);
			}
		}
	}

	VertexBuffer::~VertexBuffer()
	{
		_video->DeleteVertexBuffer(&_id);
	}

	void VertexBuffer::Bind() const
	{
		_video->BindVertexBuffer(_id);
	}

	void VertexBuffer::BufferSubData(uint32_t location, size_t size, const void* data) const
	{
		Bind();
		_video->BufferSubData(location, size, data);
	}
 
	void VertexBuffer::Unbind() const
	{
		_video->UnbindVertexBuffer();
	}
}