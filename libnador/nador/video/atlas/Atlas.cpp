#include <nlohmann/json.hpp>

#include "nador/video/atlas/Atlas.h"

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

    Atlas::Atlas(const IVideo*          video,
                 const IFileController* fileCtrl,
                 const std::string&     atlasImagePath,
                 const std::string&     textureName,
                 const std::string&     configName)
    : _video(video)
    , _fileCtrl(fileCtrl)
    , _textureName(textureName)
    , _configName(configName)
    , _atlasImagePath(atlasImagePath)
    {
        NADOR_ASSERT(_video);
        NADOR_ASSERT(_fileCtrl);

        auto configData = _fileCtrl->Read((_atlasImagePath + configName));

        nlohmann::json json = nlohmann::json::parse(configData->Begin(), configData->End());

        for (const auto& image : json["images"])
        {
            video::EImageName id        = image["id"].get<video::EImageName>();
            uint32_t          width     = image["width"].get<uint32_t>();
            uint32_t          height    = image["height"].get<uint32_t>();
            std::string       name      = image["Name"].get<std::string>();
            std::string       atlasName = image["atlas_name"].get<std::string>();

            glm::mat4x2 uvs;

            uvs[0][0] = image["UV_X_1"].get<float_t>();
            uvs[0][1] = image["UV_Y_1"].get<float_t>();

            uvs[1][0] = image["UV_X_2"].get<float_t>();
            uvs[1][1] = image["UV_Y_2"].get<float_t>();

            uvs[2][0] = image["UV_X_3"].get<float_t>();
            uvs[2][1] = image["UV_Y_3"].get<float_t>();

            uvs[3][0] = image["UV_X_4"].get<float_t>();
            uvs[3][1] = image["UV_Y_4"].get<float_t>();

            _images[id] = std::make_unique<Image>(width, height, name, atlasName, id, uvs);
            _imageIds.push_back(id);
            _imageNames.push_back(name);
        }
    }

    void Atlas::LoadTexture()
    {
        if (IsLoadedTexture())
        {
            return;
        }

        auto textureData = _fileCtrl->Read((_atlasImagePath + _textureName));

        _texture.reset(new Texture(_video, textureData));
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