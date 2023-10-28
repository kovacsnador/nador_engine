#ifndef __ATLAS_H__
#define __ATLAS_H__

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

#include "EImageName.h"
#include "nador/video/buffers/Texture.h"
#include "nador/video/atlas/AtlasConfigParser.h"

namespace nador
{
    struct Image
    {
        uint32_t          width { 0 };
        uint32_t          height { 0 };
        std::string       name;
        std::string       atlasName;
        video::EImageName id { video::EImageName::IMAGE_NAME_COUNT };
        glm::mat4x2       uvs;
    };

    struct ImageData
    {
        const Image*     image;
        const TexturePtr texture;
    };

    class Atlas
    {
        using image_container_t = std::unordered_map<video::EImageName, std::unique_ptr<Image>>;

    public:
        using image_ids_t                 = std::vector<video::EImageName>;
        using TextureFileLoaderStrategy_t = std::function<std::unique_ptr<Texture>(const std::filesystem::path&)>;

        /*!
         * Atlas constructor.
         */
        Atlas(const std::filesystem::path& atlasImagePath, const std::vector<Image>& images, const TextureFileLoaderStrategy_t& textureLoader);

        /*!
         * Loads the texture.
         */
        void LoadTexture();

        /*!
         * Deloads the texture.
         */
        void DeloadTexture();

        /*!
         * Checks if the texture is loaded.
         *
         * \return	True if loaded, false otherwise
         */
        bool IsLoadedTexture() const;

        /*!
         * Gets the image data.
         *
         * \param name	The image id.
         *
         * \return	Pointer of the image data, or nullptr.
         */
        const Image* GetImage(video::EImageName name) const;

        /*!
         * Gets the image data.
         *
         * \param name	The image id.
         *
         * \return	The Image data.
         */
        ImageData GetImageData(video::EImageName name);

        /*!
         * Gets all image ids in atlas.
         *
         * \return	The image ids container.
         */
        const image_ids_t& GetImageIds() const;

        /*!
         * Gets all image names in atlas.
         *
         * \return	The image names container.
         */
        const strings_t& GetImageNames() const;

        /*!
         * Gets texture name.
         *
         * \return	The texture file name.
         */
        std::string GetTextureName() const;

        /*!
         * Gets config name.
         *
         * \return	The config file name.
         */
        // const std::string& GetConfigName() const;

    private:
        std::filesystem::path    _atlasTexturePath;
        std::shared_ptr<Texture> _texture;
        image_container_t        _images;
        image_ids_t              _imageIds;
        strings_t                _imageNames;

        TextureFileLoaderStrategy_t _textureLoader;
    };

    CREATE_PTR_TYPES(Atlas);
} // namespace nador

#endif // !__ATLAS_H__
