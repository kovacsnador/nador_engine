#include "stb_image/stb_image.h"

#include "nador/video/TextureLoader.h"
#include "nador/log/Log.h"
#include "nador/App.h"

namespace nador
{
	TextureData::TextureData(const char* filePath)
	{
		NADOR_ASSERT(filePath);

		IApp* app = IApp::Get();

		IFileController* fileCtrl = app->GetFileController();
		DataPtr file = fileCtrl->Read(filePath);

		_CreateTextureData(file);
	}

	TextureData::TextureData(const DataPtr& data)
	{
		_CreateTextureData(data);
	}

	TextureData::~TextureData()
	{
		if(localBuffer)
		{
			stbi_image_free(localBuffer);
			localBuffer = nullptr;
		}
		width = 0;
		height = 0;
		pbb = 0;
	}

	void TextureData::_CreateTextureData(const DataPtr& data)
	{
		NADOR_ASSERT(data->IsValid());

		stbi_set_flip_vertically_on_load(1);
		localBuffer = stbi_load_from_memory((stbi_uc*)data->GetData(), data->GetSize(), &width, &height, &pbb, 4);
	}

	TextureDataPtr TextureLoader::LoadFromFile(const char* filePath)
	{
		return std::make_shared<TextureData>(filePath);
	}

	TextureDataPtr TextureLoader::LoadFromBuffer(const DataPtr& data)
	{
		return std::make_shared<TextureData>(data);
	}
}