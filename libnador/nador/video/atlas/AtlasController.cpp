#include <nlohmann/json.hpp>

#include "nador/video/atlas/AtlasController.h"
#include "nador/App.h"

namespace nador
{
	AtlasController::AtlasController(const IVideo* video, const IFileController* fileCtrl, const DataPtr& fileData, const std::string& atlasImagesPath)
	{
		NADOR_ASSERT(video);
		NADOR_ASSERT(fileData);

		if(fileData->IsValid())
		{
			nlohmann::json json = nlohmann::json::parse(fileData->Begin(), fileData->End());
			
			for(auto& it : json["atlas_names"])
			{
				std::string imageName = it["i_name"].get<std::string>();
				std::string configName = it["c_name"].get<std::string>();

				AtlasPtr atlasPtr = std::make_shared<Atlas>(video, fileCtrl, atlasImagesPath, imageName, configName);

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