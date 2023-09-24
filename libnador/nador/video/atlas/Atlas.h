#ifndef __ATLAS_H__
#define __ATLAS_H__

#include <string>
#include <map>
#include <vector>
#include <filesystem>

#include "EImageName.h"
#include "nador/video/buffers/Texture.h"

namespace nador
{
    struct Image
    {
    public:
        /*!
         * AtlasImage default constructor.
         */
        Image() = default;

        /*!
         * AtlasImage constructor.
         *
         * \param width		The image width.
         * \param height	The image height.
         * \param name		The image name.
         * \param atlasName	The atlas name.
         * \param id		The image id.
         * \param uvs		The uv coordiantes.
         */
        Image(const uint32_t          width,
              const uint32_t          height,
              const std::string&      name,
              const std::string&      atlasName,
              const video::EImageName id,
              const glm::mat4x2&      uvs);

        uint32_t          width { 0 };
        uint32_t          height { 0 };
        std::string       name;
        std::string       atlasName;
        video::EImageName id { video::EImageName::IMAGE_NAME_COUNT };
        glm::mat4x2       uvs;
    };

    struct ImageData
    {
        const Image*      image;
        const TexturePtr& texture;
    };

    class Atlas
    {
        using image_container_t = std::map<video::EImageName, std::unique_ptr<Image>>;

    public:
        using image_ids_t = std::vector<video::EImageName>;

        /*!
         * Atlas constructor.
         *
         * \param video				The video api interface.
         * \param fileCtrl	        The file controller.
         * \param atlasImagePath	The path of the atlas images folder.
         * \param textureName		The name of the texture.
         * \param configName		The name of the config file.
         */
        Atlas(const IVideo*                video,
              const IFileController*       fileCtrl,
              const std::filesystem::path& atlasImagePath,
              const std::string&           textureName,
              const std::string&           configName);

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
        const std::string& GetTextureName() const;

        /*!
         * Gets config name.
         *
         * \return	The config file name.
         */
        const std::string& GetConfigName() const;

    private:
        const IVideo*          _video;
        const IFileController* _fileCtrl;

        std::string           _textureName;
        std::string           _configName;
        std::filesystem::path _atlasImagePath;
        TexturePtr            _texture;
        image_container_t     _images;
        image_ids_t           _imageIds;
        strings_t             _imageNames;
    };

    CREATE_PTR_TYPES(Atlas);
} // namespace nador

#endif // !__ATLAS_H__
