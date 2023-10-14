#include "nador/video/atlas/AtlasController.h"

namespace nador
{
    AtlasController::AtlasController(IAtlasController::AtlasList_t atlasList, size_t cacheSize)
    : _atlases(std::move(atlasList))
    , _cache(cacheSize, [](NADOR_MAYBE_UNUSED const auto& key, const auto& val) { val->DeloadTexture(); })
    {        
        for(size_t i = 0; i < _atlases.size(); ++i)
        {
            const auto& atlasPtr = _atlases[i];
            for (const auto& img : atlasPtr->GetImageIds())
            {
                _imageLookup[img] = i;
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

    const AtlasController::AtlasList_t& AtlasController::GetAtlases() const
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