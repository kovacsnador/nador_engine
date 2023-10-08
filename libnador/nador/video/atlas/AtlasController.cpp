#include "nador/video/atlas/AtlasController.h"

namespace nador
{
    AtlasController::AtlasController(const IVideoPtr video, const IFileControllerPtr fileCtrl, const AtlasSettings& atlasSettings, const atlas::AtlasConfigList_t& list)
    : _video(std::move(video))
    , _fileCtrl(std::move(fileCtrl))
    , _cache(atlasSettings.atlasCacheSize, [](NADOR_MAYBE_UNUSED const auto& key, const auto& val) { val->DeloadTexture(); })
    {
        NADOR_ASSERT(_video);
        NADOR_ASSERT(_fileCtrl);

        for (const auto& atlasConfig : list)
        {
            AtlasPtr atlasPtr = std::make_shared<Atlas>(_video.get(), _fileCtrl.get(), atlasSettings.atlasImagesPath, atlasConfig);
            _atlases.push_back(atlasPtr);

            for (const auto& img : atlasPtr->GetImageIds())
            {
                _imageLookup[img] = _atlases.size() - 1;
            }
        }
    }

    ImageData AtlasController::GetImage(video::EImageName name) const
    {
        auto it = _imageLookup.find(name);
        if (it == _imageLookup.end())
        {
            ENGINE_FATAL("Image id %d not found!", name);
        }

        const auto& key = it->second;

        if (_cache.Exist(key))
        {
            auto& atlas = _cache.Get(key);
            return atlas->GetImageData(name);
        }
        else
        {
            const auto& atlas = _atlases.at(key);
            _cache.Insert(key, atlas);
            return atlas->GetImageData(name);
        }
    }

    const AtlasController::atlases_t& AtlasController::GetAtlases() const
    {
        return _atlases;
    }

    strings_t AtlasController::GetAtlasNames() const
    {
        strings_t atlasNames;
        for (auto& it : _atlases)
        {
            atlasNames.push_back(it->GetTextureName());
        }
        return atlasNames;
    }
} // namespace nador