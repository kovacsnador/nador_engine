#ifndef __FONT_CONTROLLER_H__
#define __FONT_CONTROLLER_H__

#include <memory>
#include <mutex>
#include <future>
#include <unordered_map>

#include "nador/video/font/IFontController.h"
#include "nador/video/font/FontTextureLoadStrategy.h"
#include "nador/common/MoveableObjWrapper.h"
#include "nador/common/FutureWaiter.h"

namespace nador
{
    class FontController : public IFontController
    {
        using FontsList_t      = std::unordered_map<uint32_t, std::unordered_map<uint32_t, FontPtr>>;
        using FontNameToId_t   = std::unordered_map<std::string, uint32_t>;
        using PendingFutures_t = std::vector<nador::MoveableObjWrapper<std::future<void>, nador::FutureWaiter>>;

    public:
        const uint32_t INVALID_FONT_VALUE = (uint32_t)-1;

        /*!
         * Font controller default constructor.
         */
        FontController(const font::FontTextureLoadStrategy_t<Texture>& loadStrategy, uint32_t maxTextureSize);

        /*!
         * Add font size.
         *
         * \param fontSize		The actual font size.
         */
        bool AddFontSize(uint32_t fontSize) override;

        /*!
         * Creates a new font.
         *
         * \param fontId		The id of the font.
         * \param fileData		The font file data.
         */
        void CreateFont(uint32_t fontId, const std::optional<FileData>& fileData) override;

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
        const FontSizesList_t& GetFontSizes() const override;

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

        /*!
         * Waits until the pending requests are done.
         */
        void Wait() override;

    private:
        FontUPtr _CreateFont(FT_Library library, const FileData& data, uint32_t fontSize, uint32_t maxTextureSize);

        void _RegisterFont(uint32_t fontId, uint32_t fontSize, FileData file, const std::string& fileName, uint32_t maxTextureSize);

        int32_t         _maxTextureSize;
        FontsList_t     _registeredFonts;
        FontNameToId_t  _fontNamesToIds;
        FontSizesList_t _fontSizes;

        PendingFutures_t _pendingFutures;

        uint32_t _defaultFontId { INVALID_FONT_VALUE };
        uint32_t _defaultFontSize { INVALID_FONT_VALUE };

        font::FontTextureLoadStrategy_t<Texture> _fontTextureLoadStrategy;

        mutable std::mutex _mutex;
    };

    CREATE_PTR_TYPES(FontController);
} // namespace nador

#endif //!__FONT_CONTROLLER_H__
