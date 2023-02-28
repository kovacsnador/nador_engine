#include "nador/video/buffers/IndexBuffer.h"
#include "nador/log/Log.h"
#include "nador/App.h"

namespace nador
{
	IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
	: _id(0)
	, _video(IApp::Get()->GetVideo())
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