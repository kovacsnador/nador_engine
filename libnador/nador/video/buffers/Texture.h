#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <memory>

#include "nador/utils/Types.h"
#include "nador/video/IVideo.h"
#include "nador/system/IFileController.h"
#include "nador/video/TextureLoader.h"
#include "nador/utils/Utils.h"

namespace nador
{
    class Texture
    {
    public:
        /*!
         * Texture constructor.
         *
         * \param video				The video api interface.
         * \param data				The texture buffer.
         * \param textureSettings	The specified texture settings.
         */
        Texture(const IVideo* video, const FileData& data, TextureSettings textureSettings = {});

        /*!
         * Texture constructor.
         *
         * \param video				The video api interface.
         * \param data				The texture data.
         * \param width				The width of the texture.
         * \param height			The height of the texture.
         * \param textureSettings	The specified texture settings.
         */
        Texture(const IVideo* video, const void* data, uint32_t width, uint32_t height, TextureSettings textureSettings = {});

        /*!
         * Texture destructor.
         */
        ~Texture();

        /*!
         * Uploads subimage into the texture.
         *
         * \param data				The texture data.
         * \param xoffset			The x offset.
         * \param yoffset			The y offset.
         * \param width				The texture width.
         * \param height			The texture height.
         */
        void UploadSubTexture(const void* data, int32_t xOffset, int32_t yOffset, int32_t width, int32_t height);

        /*!
         * Bind the texture.
         *
         * \param slot	The texture slot.
         */
        void Bind(uint32_t slot = 0);

        /*!
         * Unbind the texture.
         */
        void Unbind();

        /*!
         * Gets the render id.
         *
         * \return	The renderer id:
         */
        uint32_t GetId() const;

        /*!
         * Gets the texture width.
         *
         * \return	The width of the texture:
         */
        int32_t GetWidth() const { return _width; }

        /*!
         * Gets the texture height.
         *
         * \return	The height of the texture:
         */
        int32_t GetHeight() const { return _height; }

    private:
        const IVideo* _video { nullptr };
        uint32_t      _id { 0 };

        int32_t _width { 0 };
        int32_t _height { 0 };
        int32_t _pbb { 0 };

        TextureSettings _textureSettings;
    };

    CREATE_PTR_TYPES(Texture);
} // namespace nador

#endif // !__TEXTURE_H__
