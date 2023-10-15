#ifndef __FONT_H__
#define __FONT_H__

#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <atomic>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "nador/utils/Types.h"
#include "nador/video/buffers/Texture.h"
#include "nador/video/renderer/RenderData.h"
#include "nador/video/font/FontTextureLoadStrategy.h"

namespace nador
{
    class Font
    {
        struct CharacterInfo
        {
            float_t ax { 0 }; // advance.x
            float_t ay { 0 }; // advance.y

            float_t bw { 0 }; // bitmap.width;
            float_t bh { 0 }; // bitmap.height;

            float_t bl { 0 }; // bitmap_left;
            float_t bt { 0 }; // bitmap_top;

            float_t tx { 0 }; // x offset of glyph in texture coordinates
            float_t ty { 0 }; // y offset of glyph in texture coordinates
        };

        struct BitmapLoadData
        {
            int32_t                    xOffset { 0 };
            int32_t                    yOffset { 0 };
            uint32_t                   bitmapWidth { 0 };
            uint32_t                   bitmapRows { 0 };
            std::unique_ptr<uint8_t[]> bitmapBuffer { nullptr };
        };

        struct TextureLoadData
        {
            uint32_t width { 0 };
            uint32_t height { 0 };

            std::vector<BitmapLoadData> bitmapLoadDatas {};
        };

    public:
        static constexpr float_t DEFAULT_NEW_LINE_OFFSET = 1.1f;

        /*!
         * Font constructor.
         */
        Font(const font::FontTextureLoadStrategy_t<Texture>& loadStrategy, const FT_Face face, uint32_t fontSize, uint32_t maxTextureSize);

        /*!
         * Get the font texture.
         */
        const TexturePtr GetTexture();

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

        uint32_t           _fontSize;
        std::atomic<State> _state;
        float_t            _maxYSize { 0 };

        std::unordered_map<uint32_t, CharacterInfo> _characters;

        font::FontTextureLoadStrategy_t<Texture> _loadStrategy;

        TexturePtr      _texture;
        TextureLoadData _textureLoadData;
    };

    CREATE_PTR_TYPES(Font);
} // namespace nador

#endif // !__FONT_H__
