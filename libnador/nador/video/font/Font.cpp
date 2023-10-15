#include <algorithm>
#include <locale>
#include <codecvt>

#include "nador/video/font/Font.h"
#include "nador/utils/Utils.h"

namespace nador
{
    Font::Font(const font::FontTextureLoadStrategy_t<Texture>& loadStrategy, const FT_Face face, uint32_t fontSize, uint32_t maxTextureSize)
    : _fontSize(fontSize)
    , _state(State::LOADING)
    , _loadStrategy(loadStrategy)
    {
        NADOR_ASSERT(_loadStrategy);

        FT_Set_Pixel_Sizes(face, 0, fontSize);
        const FT_GlyphSlot glyph = face->glyph;

        uint32_t rowWidth      = 0;
        uint32_t rowHeight     = 0;
        uint32_t textureWidth  = 0;
        uint32_t textureHeight = 0;

        uint32_t g_index;

        size_t charCount = 0;

        /* Find minimum size for a texture holding all visible ASCII characters */
        uint32_t charCode = FT_Get_First_Char(face, &g_index);
        while (g_index != 0)
        {
            if (FT_Load_Char(face, charCode, FT_LOAD_BITMAP_METRICS_ONLY | FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT))
            {
                ENGINE_WARNING("Loading characher failed! char: %d", charCode);
                continue;
            }

            if (rowWidth + glyph->bitmap.width + 1 >= maxTextureSize)
            {
                textureWidth = std::max(textureWidth, rowWidth);
                textureHeight += rowHeight;
                rowWidth  = 0;
                rowHeight = 0;
            }
            rowWidth += glyph->bitmap.width + 1;
            rowHeight = std::max(rowHeight, glyph->bitmap.rows);

            charCode = FT_Get_Next_Char(face, charCode, &g_index);
            charCount++;
        }

        _textureLoadData.bitmapLoadDatas.reserve(charCount);

        textureWidth = std::max(textureWidth, rowWidth);
        textureHeight += rowHeight;

        textureHeight = utils::NextUpPower(textureHeight, 2u);
        textureWidth  = utils::NextUpPower(textureWidth, 2u);

        if (textureHeight > maxTextureSize || textureWidth > maxTextureSize)
        {
            ENGINE_FATAL("Font texture size is to big for opengl");
            _state.store(State::FAILED);
        }

        _textureLoadData.width  = textureWidth;
        _textureLoadData.height = textureHeight;

        /* Paste all glyph bitmaps into the texture, remembering the offset */
        int32_t ox = 0;
        int32_t oy = 0;

        rowHeight = 0;

        charCode = FT_Get_First_Char(face, &g_index);
        while (g_index != 0)
        {
            if (FT_Load_Char(face, charCode, FT_LOAD_RENDER))
            {
                ENGINE_WARNING("Loading character %c failed!\n", charCode);
                continue;
            }

            if (ox + glyph->bitmap.width + 1 >= maxTextureSize)
            {
                oy += rowHeight;
                rowHeight = 0;
                ox        = 0;
            }

            uint32_t bitmapBufferSize = glyph->bitmap.width * glyph->bitmap.rows;

            std::unique_ptr<uint8_t[]> bitmapBuffer = std::make_unique<uint8_t[]>(bitmapBufferSize);
            std::memcpy(bitmapBuffer.get(), glyph->bitmap.buffer, bitmapBufferSize);

            _textureLoadData.bitmapLoadDatas.emplace_back(ox, oy, glyph->bitmap.width, glyph->bitmap.rows, std::move(bitmapBuffer));

            CharacterInfo c;

            c.ax = (float_t)(glyph->advance.x >> 6);
            c.ay = (float_t)(glyph->advance.y >> 6);

            c.bw = (float_t)glyph->bitmap.width;
            c.bh = (float_t)glyph->bitmap.rows;

            c.bl = (float_t)glyph->bitmap_left;
            c.bt = (float_t)glyph->bitmap_top;

            c.tx = ox / (float_t)textureWidth;
            c.ty = oy / (float_t)textureHeight;

            _characters[charCode] = c;

            rowHeight = std::max(rowHeight, glyph->bitmap.rows);
            ox += glyph->bitmap.width + 1;

            charCode = FT_Get_Next_Char(face, charCode, &g_index);
        }

        // Setup max height
        std::wstring ws(L"AgyŰ");

        // setup converter
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;
        std::string                                 bigText = converter.to_bytes(ws);
        _maxYSize                                           = GetBoundingBox(CalculateUTF8TextVertices(bigText.c_str())).y;

        _state.store(State::FINISHED);

        ENGINE_DEBUG("Generated a %d x %d (%d kb) texture atlas", textureWidth, textureHeight, textureWidth * textureHeight / maxTextureSize);
    }

    float_t Font::GetMaxHeight() const noexcept
    {
        return _maxYSize;
    }

    const TexturePtr Font::GetTexture()
    {
        _LoadTextureIfNecessary();
        return _texture;
    }

    RenderData Font::CalculateUTF8Text(const char* utf8Text, float_t newLineOffset)
    {
        std::u32string utf32Text = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> {}.from_bytes(utf8Text);
        return CalculateUTF32Text(utf32Text.c_str(), newLineOffset);
    }

    RenderData Font::CalculateUTF16Text(const char* utf16Text, float_t newLineOffset)
    {
        std::u32string utf32Text = std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> {}.from_bytes(utf16Text);
        return CalculateUTF32Text(utf32Text.c_str(), newLineOffset);
    }

    RenderData Font::CalculateUTF32Text(const char32_t* text, float_t newLineOffset)
    {
        NADOR_ASSERT(text);

        _LoadTextureIfNecessary();

        RenderData renderData;

        const float_t sx = 0.03f;
        const float_t sy = 0.03f;

        float_t x = 0;
        float_t y = 0;

        uint32_t i = 0;

        int32_t texture_w = _texture->GetWidth();
        int32_t texture_h = _texture->GetHeight();

        std::vector<float_t>  vertices(16, 0);
        std::vector<float_t>  texCoords(8, 0);
        std::vector<uint32_t> indices(6, 0);

        for (const char32_t* p = text; *p; p++)
        {
            try
            {
                if (*p == '\n')
                {
                    x = 0;
                    y -= _fontSize * sx * newLineOffset;
                    continue;
                }

                auto iterator = _characters.find(*p);

                bool isCharSpace = isspace(*p);

                if (iterator == _characters.end())
                {
                    if (isCharSpace == false)
                    {
                        ENGINE_WARNING("Can't find character in map: %d", *p);
                    }
                    continue;
                }

                const CharacterInfo& c_info = iterator->second;

                // Calculate the vertex and texture coordinates
                float_t x2     = x + c_info.bl * sx;
                float_t y2     = -y - c_info.bt * sy;
                float_t width  = c_info.bw * sx;
                float_t height = c_info.bh * sy;

                // Advance the cursor to the start of the next character
                x += c_info.ax * sx;
                y += c_info.ay * sy;

                // Skip glyphs that have no pixels
                if (!width || !height)
                {
                    if (isCharSpace)
                    {
                        width = c_info.ax * sx;
                    }
                    else
                    {
                        ENGINE_WARNING("Can't find glyphs: %d", *p);
                        continue;
                    }
                }

                vertices = { x2, -y2, 0.0f, 1.0f, x2 + width, -y2, 0.0f, 1.0f, x2, -y2 - height, 0.0f, 1.0f, x2 + width, -y2 - height, 0.0f, 1.0f };

                texCoords = { c_info.tx,
                              c_info.ty,
                              c_info.tx + c_info.bw / texture_w,
                              c_info.ty,
                              c_info.tx,
                              c_info.ty + c_info.bh / texture_h,
                              c_info.tx + c_info.bw / texture_w,
                              c_info.ty + c_info.bh / texture_h };

                indices = { i, i + 1, i + 2, i + 1, i + 3, i + 2 };

                renderData.AddVertices(&vertices[0], utils::VectorsizeInBytes(vertices));
                renderData.AddTextureCoords(&texCoords[0], utils::VectorsizeInBytes(texCoords));
                renderData.AddIndices(&indices[0], utils::VectorsizeInBytes(indices));

                i += 4;
            }
            catch (...)
            {
                ENGINE_WARNING("Can't find glyphs: %d", *p);
                continue;
            }
        }
        return renderData;
    }

    vertices_list_t Font::CalculateUTF8TextVertices(const char* text, float_t newLineOffset)
    {
        std::u32string utf32Text = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> {}.from_bytes(text);
        return CalculateUTF32TextVertices(utf32Text.c_str(), newLineOffset);
    }

    vertices_list_t Font::CalculateUTF16TextVertices(const char* text, float_t newLineOffset)
    {
        std::u32string utf32Text = std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> {}.from_bytes(text);
        return CalculateUTF32TextVertices(utf32Text.c_str(), newLineOffset);
    }

    vertices_list_t Font::CalculateUTF32TextVertices(const char32_t* text, float_t newLineOffset)
    {
        NADOR_ASSERT(text);

        RenderData renderData;

        const float_t sx = 0.03f;
        const float_t sy = 0.03f;

        float_t x = 0;
        float_t y = 0;

        uint32_t i = 0;

        std::vector<float_t> vertices(16, 0);

        for (const char32_t* p = text; *p; p++)
        {
            try
            {
                if (*p == 10)
                {
                    x = 0;
                    y -= _fontSize * sx * newLineOffset;
                    continue;
                }

                auto iterator = _characters.find(*p);

                if (iterator == _characters.end())
                {
                    ENGINE_WARNING("Can't find character in map: %d", *p);
                    continue;
                }

                const CharacterInfo& c_info = iterator->second;

                // Calculate the vertex and texture coordinates
                float_t x2     = x + c_info.bl * sx;
                float_t y2     = -y - c_info.bt * sy;
                float_t width  = c_info.bw * sx;
                float_t height = c_info.bh * sy;

                // Advance the cursor to the start of the next character
                x += c_info.ax * sx;
                y += c_info.ay * sy;

                // Skip glyphs that have no pixels
                if (!width || !height)
                {
                    if (isspace(*p) == false)
                    {
                        ENGINE_WARNING("Can't find glyphs: %d", *p);
                    }
                    continue;
                }

                vertices = { x2, -y2, 0.0f, 1.0f, x2 + width, -y2, 0.0f, 1.0f, x2, -y2 - height, 0.0f, 1.0f, x2 + width, -y2 - height, 0.0f, 1.0f };

                renderData.AddVertices(&vertices[0], utils::VectorsizeInBytes(vertices));

                i += 4;
            }
            catch (...)
            {
                ENGINE_WARNING("Can't find glyphs: %d", *p);
                continue;
            }
        }
        return renderData.vertices;
    }

    void Font::_LoadTextureIfNecessary()
    {
        if (_texture == nullptr && _textureLoadData.Empty() == false)
        {
            if (_state.load() == State::FAILED)
            {
                ENGINE_FATAL("Font loading failed.");
                return;
            }

            if (_state.load() == State::LOADING)
            {
                ENGINE_WARNING("Font is still loading...");
                return;
            }

            _texture = _loadStrategy(_textureLoadData.width, _textureLoadData.height);

            for (const auto& it : _textureLoadData.bitmapLoadDatas)
            {
                _texture->UploadSubTexture(it.bitmapBuffer.get(), it.xOffset, it.yOffset, it.bitmapWidth, it.bitmapRows);
            }

            // free up memory
            _textureLoadData = {};
        }
    }
} // namespace nador