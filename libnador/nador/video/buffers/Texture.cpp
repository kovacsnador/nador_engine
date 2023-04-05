#include "nador/video/buffers/Texture.h"
#include "nador/App.h"

namespace nador
{
	/*Texture::Texture(const IVideo* video, const char* filePath, TextureSettings textureSettings)
	: _video(video)
	, _id(0)
	, _width(0)
	, _height(0)
	, _pbb(0)
	, _textureSettings(textureSettings)
	{
		NADOR_ASSERT(_video);

		TextureDataPtr textureData = TextureLoader::LoadFromFile(filePath);
		_UploadTexture(textureData);
	}*/

	Texture::Texture(const IVideo* video, const DataPtr& data, TextureSettings textureSettings)
	: _video(video)
	, _id(0)
	, _width(0)
	, _height(0)
	, _pbb(0)
	, _textureSettings(textureSettings)
	{
		TextureDataPtr textureData = TextureLoader::LoadFromBuffer(data);
		_UploadTexture(textureData);
	}

	Texture::Texture(const IVideo* video, const void* data, uint32_t width, uint32_t height, TextureSettings textureSettings)
	: _video(video)
	, _id(0)
	, _width(width)
	, _height(height)
	, _pbb(0)
	, _textureSettings(textureSettings)
	{
		_id = _video->UploadTexture(data, height, width, textureSettings);
	}

	Texture::~Texture()
	{
 		_video->DeleteTexture(&_id);
	}

	void Texture::UploadSubTexture(const void* data, int32_t xOffset, int32_t yOffset, int32_t width, int32_t height)
	{
		Bind();
		_video->UploadSubTexture(data, xOffset, yOffset, width, height, _textureSettings);
		Unbind();
	}

	void Texture::Bind(uint32_t slot)
	{
		_video->BindTexture(_id, slot);
	}

	void Texture::Unbind()
	{
		_video->UnbindTexture();
	}

	uint32_t Texture::GetId() const
	{
		return _id;
	}

	void Texture::_UploadTexture(const TextureDataPtr& textureData)
	{
		_id = _video->UploadTexture(textureData->localBuffer, textureData->height, textureData->width, _textureSettings);

		_width = textureData->width;
		_height = textureData->height;
		_pbb = textureData->pbb;
	}
}