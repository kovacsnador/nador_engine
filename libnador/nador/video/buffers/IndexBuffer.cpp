#include "nador/video/buffers/IndexBuffer.h"
#include "nador/log/ILog.h"

namespace nador
{
	IndexBuffer::IndexBuffer(const IVideo* video, const uint32_t* data, uint32_t count)
	: _id(0)
	, _video(video)
	, _count(count)
	{
		NADOR_ASSERT(_video && data);
		_id = _video->UploadIndexBuffer(data, count);
	}

	IndexBuffer::~IndexBuffer()
	{
		_video->DeleteIndexBuffer(&_id);
	}

	void IndexBuffer::Bind() const
	{
		_video->BindIndexBuffer(_id);
	}

	void IndexBuffer::Unbind() const
	{
		_video->UnbindIndexBuffer();
	}

	uint32_t IndexBuffer::GetCount()
	{
		return _count;
	}
}