#ifndef __I_FONT_CONTROLLER_H__
#define __I_FONT_CONTROLLER_H__

#include <set>

#include "nador/utils/Types.h"
#include "nador/video/font/Font.h"

namespace nador
{
    class IFontController
    {
    public:
        virtual ~IFontController() = default;

    public:
        using FontSizesList_t = std::set<uint32_t>;

        virtual bool AddFontSize(uint32_t fontSize) = 0;

        virtual void CreateFont(uint32_t fontId, const std::optional<FileData>& fileData) = 0;

        virtual const FontPtr GetFont(uint32_t fontId, uint32_t fontSize) const = 0;

        virtual strings_t GetRegisteredFontNames() const = 0;

        virtual uint32_t GetFontIdByName(const std::string& fontName) const = 0;

        virtual strings_t GetFontSizesAsString() const = 0;

        virtual const FontSizesList_t& GetFontSizes() const = 0;

        virtual void SetDefaultSystemFont(uint32_t fontId, uint32_t fontSize) = 0;

        virtual const FontPtr GetDefaultSystemFont() const = 0;

        virtual bool IsLoading() const = 0;

        virtual void Wait() = 0;
    };
    CREATE_PTR_TYPES(IFontController);
} // namespace nador

#endif // !__I_FONT_CONTROLLER_H__
