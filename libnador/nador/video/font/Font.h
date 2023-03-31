#ifndef __FONT_H__
#define __FONT_H__

#include <map>
#include <unordered_map>
#include <vector>
#include <memory>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "nador/utils/Types.h"
#include "nador/video/buffers/Texture.h"
#include "nador/video/renderer/RenderData.h"

namespace nador
{
    class Font
    {
        struct CharacterInfo
        {

            /**
             * CharacterInfo default constructor
             */
            CharacterInfo()
                : ax(0)
                , ay(0)
                , bw(0)
                , bh(0)
                , bl(0)
                , bt(0)
                , tx(0)
                , ty(0)
            {
                // no func
            }

            float_t ax;	// advance.x
            float_t ay;	// advance.y

            float_t bw;	// bitmap.width;
            float_t bh;	// bitmap.height;

            float_t bl;	// bitmap_left;
            float_t bt;	// bitmap_top;

            float_t tx;	// x offset of glyph in texture coordinates
            float_t ty;	// y offset of glyph in texture coordinates
        };

        using char_container_t = std::unordered_map<uint32_t, CharacterInfo>;

        struct BitmapLoadData
        {
            BitmapLoadData() = default;

            BitmapLoadData(int32_t xOffset, int32_t yOffset, uint32_t bitmapWidth, uint32_t bitmapRows, std::unique_ptr<uint8_t[]> bitmapBuffer)
            : xOffset(xOffset)
            , yOffset(yOffset)
            , bitmapWidth(bitmapWidth)
            , bitmapRows(bitmapRows)
            , bitmapBuffer(std::move(bitmapBuffer))
            {
            }

            int32_t xOffset = 0;
            int32_t yOffset = 0;
            uint32_t bitmapWidth = 0;
            uint32_t bitmapRows = 0;
            std::unique_ptr<uint8_t[]> bitmapBuffer = nullptr;
        };

        struct TextureLoadData
        {
            uint32_t width;
            uint32_t height;
        };

        using bitmap_load_datas_t = std::vector<BitmapLoadData>;

    public:
        static constexpr float_t DEFAULT_NEW_LINE_OFFSET = 1.1f;

        /*!
         * Font constructor.
         *
         * \param face		    The font face.
         * \param fontSize	    The size of the font.
         * \param fileName	    The name of the font file.
         */
        Font(const IVideo* video, const FT_Face face, uint32_t fontSize, uint32_t maxTextureSize, const char* fileName);

        /*!
         * Get the font texture.
         */
        const TexturePtr GetTexture();

        /*!
         * Get the font file name.
         */
        const std::string& GetFileName() const;

        /*!
         * Calculate the render data of the utf8 text.
         *
         * \param text		The text to render.
         *
         * \return      The render data for the text.
         */
        RenderData CalculateUTF8Text(const char* text, float_t newLineOffset = DEFAULT_NEW_LINE_OFFSET);

        /*!
         * Calculate the render data of the utf16 text.
         *
         * \param text		The text to render.
         *
         * \return      The render data for the text.
         */
        RenderData CalculateUTF16Text(const char* text, float_t newLineOffset = DEFAULT_NEW_LINE_OFFSET);

        /*!
         * Calculate the render data of the utf32 text.
         *
         * \param text		The text to render.
         *
         * \return      The render data for the text.
         */
        RenderData CalculateUTF32Text(const char32_t* text, float_t newLineOffset = DEFAULT_NEW_LINE_OFFSET);

        vertices_list_t CalculateUTF8TextVertices(const char* text, float_t newLineOffset = DEFAULT_NEW_LINE_OFFSET);
        vertices_list_t CalculateUTF16TextVertices(const char* text, float_t newLineOffset = DEFAULT_NEW_LINE_OFFSET);
        vertices_list_t CalculateUTF32TextVertices(const char32_t* text, float_t newLineOffset = DEFAULT_NEW_LINE_OFFSET);

        float_t GetMaxHeight() const noexcept;

    private:
        /*!
         * Loads the texture if it's not loaded.
         */
        void _LoadTextureIfNecessary();

        enum class State
        {
            LOADING,
            FINISHED,
            FAILED,
        };

        const IVideo* _video;

        uint32_t            _fontSize;
        char_container_t    _characters;
        TexturePtr          _texture;
        State               _state;
        std::string         _fileName;
        float_t             _maxYSize{ 0 };

        TextureLoadData     _textureLoadData;
        bitmap_load_datas_t _bitmapLoadDatas;
        
    };

    CREATE_PTR_TYPES(Font);
}

#endif // !__FONT_H__
