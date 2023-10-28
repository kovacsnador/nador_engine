#include <nlohmann/json.hpp>

#include "nador/video/atlas/Atlas.h"
#include "nador/log/ILog.h"

namespace nador
{
    Atlas::Atlas(const std::filesystem::path& atlasTexturePath, const std::vector<Image>& images, const TextureFileLoaderStrategy_t& textureLoader)
    : _atlasTexturePath(atlasTexturePath)
    , _textureLoader(textureLoader)
    {
        for (const auto& it : images)
        {
            _images[it.id] = std::make_unique<Image>(it);
            _imageIds.push_back(it.id);
            _imageNames.push_back(it.name);
        }
    }

    void Atlas::LoadTexture()
    {
        if (IsLoadedTexture())
        {
            return; // Texture is already loaded.
        }

        if (_textureLoader)
        {
            _texture = _textureLoader(_atlasTexturePath);
            if (_texture == nullptr)
            {
                ENGINE_ERROR("Atlas texture could not be loaded: %s", _atlasTexturePath.string().c_str());
            }
            else
            {
                ENGINE_DEBUG("Atlas texture loaded: %s", _atlasTexturePath.string().c_str());
            }
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

        ENGINE_ERROR("Image is not found in Atlas: %s", _atlasTexturePath.filename().string().c_str());
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

    std::string Atlas::GetTextureName() const
    {
        return _atlasTexturePath.filename().string();
    }
} // namespace nador