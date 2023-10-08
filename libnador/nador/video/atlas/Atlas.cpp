#include <nlohmann/json.hpp>

#include "nador/video/atlas/Atlas.h"
#include "nador/log/ILog.h"

namespace nador
{
    Image::Image(const uint32_t          width,
                 const uint32_t          height,
                 const std::string&      name,
                 const std::string&      atlasName,
                 const video::EImageName id,
                 const glm::mat4x2&      uvs)
    : width(width)
    , height(height)
    , name(name)
    , atlasName(atlasName)
    , id(id)
    , uvs(uvs)
    {
    }

    Atlas::Atlas(const IVideo*                video,
                 const IFileController*       fileCtrl,
                 const std::filesystem::path& atlasImagePath,
                 const atlas::AtlasConfig&    baseConfig)
    : _video(video)
    , _fileCtrl(fileCtrl)
    , _textureName(baseConfig.image)
    , _configName(baseConfig.config)
    , _atlasImagePath(atlasImagePath)
    {
        NADOR_ASSERT(_video);
        NADOR_ASSERT(_fileCtrl);

        auto configData = _fileCtrl->Read(_atlasImagePath / _configName);

        if (configData.has_value() == false)
        {
            throw std::runtime_error("Atlas config data could not be opened!");
        }

        auto imageConfigs = atlas::AtlasConfigParser::ParseAtlasImageConfigs<video::EImageName>(configData);

        for (const auto& it : imageConfigs)
        {
            _images[it.id] = std::make_unique<Image>(it.width, it.height, it.name, it.atlasName, it.id, it.uvs);
            _imageIds.push_back(it.id);
            _imageNames.push_back(it.name);
        }
    }

    void Atlas::LoadTexture()
    {
        if (IsLoadedTexture())
        {
            return;
        }

        auto textureData = _fileCtrl->Read(_atlasImagePath / _textureName);

        if (textureData)
        {
            _texture.reset(new Texture(_video, textureData.value()));
        }
        else
        {
            ENGINE_ERROR("Atlas texture could not be loaded: %s", _textureName.c_str());
        }
    }

    void Atlas::DeloadTexture()
    {
        _texture.reset();
    }

    bool Atlas::IsLoadedTexture() const
    {
        return _texture != nullptr;
    }

    const Image* Atlas::GetImage(video::EImageName name) const
    {
        if (_images.find(name) != _images.end())
        {
            return _images.at(name).get();
        }
        return nullptr;
    }

    ImageData Atlas::GetImageData(video::EImageName name)
    {
        LoadTexture();
        return { GetImage(name), _texture };
    }

    const Atlas::image_ids_t& Atlas::GetImageIds() const
    {
        return _imageIds;
    }

    const strings_t& Atlas::GetImageNames() const
    {
        return _imageNames;
    }

    const std::string& Atlas::GetTextureName() const
    {
        return _textureName;
    }

    const std::string& Atlas::GetConfigName() const
    {
        return _configName;
    }
} // namespace nador