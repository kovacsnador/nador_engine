#ifndef __FONT_CONTROLLER_H__
#define __FONT_CONTROLLER_H__

#include <memory>
#include <mutex>
#include <future>

#include "nador/video/font/IFontController.h"

namespace nador
{
    class FontController : public IFontController
    {
        using fonts_list_t      = std::map<uint32_t, std::map<uint32_t, FontPtr>>;
        using font_name_to_id_t = std::map<std::string, uint32_t>;
        using pending_futures_t = std::vector<std::future<void>>;

    public:
        const uint32_t INVALID_FONT_VALUE = (uint32_t)-1;

        /*!
         * Font controller default constructor.
         *
         * \param video		The video api interface.
         */
        FontController(const IVideoPtr video, const IFileControllerPtr fileCtrl);

        /*!
         * Add font size.
         *
         * \param fontSize		The actual font size.
         */
        void AddFontSize(uint32_t fontSize) override;

        /*!
         * Register a new font.
         *
         * \param fontId		The id of the font.
         * \param filePath		The path of the font file.
         */
        void RegisterFont(uint32_t fontId, std::string_view filePath) override;

        /*!
         * Gets the font.
         *
         * \param fontId		The id of the font.
         * \param fontSize		The size of the font.
         * \return	The font.
         */
        const FontPtr GetFont(uint32_t fontId, uint32_t fontSize) const override;

        /*!
         * Gets all the registered font names.
         *
         * \return	The font names.
         */
        strings_t GetRegisteredFontNames() const override;

        /*!
         * Gets the font id via font name.
         *
         * \param fontName		The name of the font.
         * \return	The font id.
         */
        uint32_t GetFontIdByName(const std::string& fontName) const override;

        /*!
         * Gets the font sizes as strings.
         *
         * \return	The font sizes as strings.
         */
        strings_t GetFontSizesAsString() const override;

        /*!
         * Gets the font sizes.
         *
         * \return	The font sizes.
         */
        const font_sizes_t& GetFontSizes() const override;

        /*!
         * Sets the default system font for debug purpose.
         *
         * \param fontId		The id of the font.
         * \param fontSize		The size of the font.
         */
        void SetDefaultSystemFont(uint32_t fontId, uint32_t fontSize) override;

        /*!
         * Gets the default system font for debug purpose.
         */
        const FontPtr GetDefaultSystemFont() const override;

        /*!
         * Checks if there is some background process.
         */
        bool IsLoading() const override;

    private:
        void _AddFont(FT_Library       library,
                      const DataPtr&   data,
                      uint32_t         fontId,
                      uint32_t         fontSize,
                      std::string_view filePath,
                      uint32_t         maxTextureSize);

        /*!
         * Register a new font.
         *
         * \param fontId		The id of the font.
         * \param fileName		The name of the font file.
         * \param maxTextureSize		The max texture size.
         */
        void _RegisterFont(uint32_t fontId, std::string filePath, uint32_t maxTextureSize);

        const IVideoPtr          _video;
        const IFileControllerPtr _fileCtrl;

        int32_t           _maxTextureSize;
        fonts_list_t      _registeredFonts;
        font_name_to_id_t _fontNamesToIds;
        font_sizes_t      _fontSizes;

        pending_futures_t _pending_futures;

        uint32_t _defaultFontId { INVALID_FONT_VALUE };
        uint32_t _defaultFontSize { INVALID_FONT_VALUE };

        mutable std::mutex _mutex;
    };

    CREATE_PTR_TYPES(FontController);
} // namespace nador

#endif //!__FONT_CONTROLLER_H__
