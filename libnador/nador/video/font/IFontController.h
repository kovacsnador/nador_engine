#ifndef __I_FONT_CONTROLLER_H__
#define __I_FONT_CONTROLLER_H__

#include "nador/utils/Types.h"
#include "nador/video/font/Font.h"

namespace nador
{
	class IFontController
	{
	public:
		virtual ~IFontController() = default;

	public:
		using font_sizes_t = std::vector<uint32_t>;

		virtual void AddFontSize(uint32_t fontSize) = 0;
		virtual void RegisterFont(uint32_t fontId, std::string_view fileName) = 0;
		virtual const FontPtr GetFont(uint32_t fontId, uint32_t fontSize) const = 0;
		virtual strings_t GetRegisteredFontNames() const = 0;
		virtual uint32_t GetFontIdByName(const std::string& fontName) const = 0;
		virtual strings_t GetFontSizesAsString() const = 0;
		virtual const font_sizes_t& GetFontSizes() const = 0;
		virtual void SetDefaultSystemFont(uint32_t fontId, uint32_t fontSize) = 0;
		virtual const FontPtr GetDefaultSystemFont() const = 0;
		virtual bool IsLoading() const = 0;
	};
	CREATE_PTR_TYPES(IFontController);
}

#endif // !__I_FONT_CONTROLLER_H__

