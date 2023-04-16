#include <nlohmann/json.hpp>

#include "nador/video/atlas/AtlasController.h"

namespace nador
{
	AtlasController::AtlasController(const IVideoPtr video, const IFileControllerPtr fileCtrl, const AtlasSettings& atlasSettigns)
	: _video(std::move(video))
	, _fileCtrl(std::move(fileCtrl))
	{
		NADOR_ASSERT(_video);
		NADOR_ASSERT(_fileCtrl);

		DataPtr atlasConfigData = fileCtrl->Read(atlasSettigns.atlasConfigPath);

		if(atlasConfigData->IsValid())
		{
			nlohmann::json json = nlohmann::json::parse(atlasConfigData->Begin(), atlasConfigData->End());
			
			for(auto& it : json["atlas_names"])
			{
				std::string imageName = it["i_name"].get<std::string>();
				std::string configName = it["c_name"].get<std::string>();

				AtlasPtr atlasPtr = std::make_shared<Atlas>(_video.get(), _fileCtrl.get(), atlasSettigns.atlasImagesPath, imageName, configName);

				_atlases.push_back(atlasPtr);

				Atlas::image_ids_t images = atlasPtr->GetImageIds();

				for(const auto& img : images)
				{
					_imageLookup[img] = _atlases.size() - 1;
				}
			}
		}
	}

	ImageData AtlasController::GetImage(video::EImageName name) const
	{
		auto it = _imageLookup.find(name);
		if(it == _imageLookup.end())
		{
			ENGINE_FATAL("Image id %d not found!", name);
		}
		return _atlases.at(it->second)->GetImageData(name);
	}

	const AtlasController::atlases_t& AtlasController::GetAtlases() const
	{
		return _atlases;
	}

	strings_t AtlasController::GetAtlasNames() const
	{
		strings_t atlasNames;
		for(auto& it : _atlases)
		{
			atlasNames.push_back(it->GetTextureName());
		}
		return atlasNames;
	}
}