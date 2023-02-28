#include "nador/video/font/FontController.h"
#include "nador/App.h"

namespace nador
{
	FontController::FontController(int32_t maxTextureSize)
	: _maxTextureSize(maxTextureSize)
	{
	}

	void FontController::AddFontSize(uint32_t fontSize)
	{
		_fontSizes.push_back(fontSize);

		//make unique the vector
		std::sort(_fontSizes.begin(), _fontSizes.end());
		_fontSizes.erase(std::unique(_fontSizes.begin(), _fontSizes.end()), _fontSizes.end());
	}

	void FontController::RegisterFont(uint32_t fontId, const char* fileName)
	{
		if(_fontSizes.empty())
		{
			ENGINE_FATAL("No font size registered! Use AddFontSize before!");
		}

		std::future<void> future = std::async(std::launch::async, &FontController::_RegisterFont, this, fontId, fileName);
		_pending_futures.push_back(std::move(future));
	}

	const FontPtr FontController::GetFont(uint32_t fontId, uint32_t fontSize) const
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if(_registeredFonts.find(fontSize) != _registeredFonts.end())
		{
			if(_registeredFonts.at(fontSize).find(fontId) != _registeredFonts.at(fontSize).end())
			{
				return _registeredFonts.at(fontSize).at(fontId);
			}
		}
		ENGINE_WARNING("Font %d not found with size: %d!", fontId, fontSize);
		return nullptr;
	}

	strings_t FontController::GetRegisteredFontNames() const
	{
		std::lock_guard<std::mutex> lock(_mutex);

		strings_t fontNames;
		for(const auto& it : _fontNamesToIds)
		{
			fontNames.push_back(it.first);
		}

		return fontNames;
	}

	uint32_t FontController::GetFontIdByName(const std::string& fontName) const
	{
		std::lock_guard<std::mutex> lock(_mutex);

		auto iterator = _fontNamesToIds.find(fontName);
		if(iterator != _fontNamesToIds.end())
		{
			return iterator->second;
		}

		return INVALID_FONT_VALUE;
	}

	strings_t FontController::GetFontSizesAsString() const
	{
		strings_t fontSizesStr;
		for(const auto& it : _fontSizes)
		{
			fontSizesStr.push_back(std::to_string(it));
		}

		return fontSizesStr;
	}

	const FontController::font_sizes_t& FontController::GetFontSizes() const
	{
		return _fontSizes;
	}

	void FontController::SetDefaultSystemFont(uint32_t fontId, uint32_t fontSize)
	{
		_defaultFontSize = fontSize;
		_defaultFontId = fontId;
	}

	const FontPtr FontController::GetDefaultSystemFont() const
	{
		if (_defaultFontSize != INVALID_FONT_VALUE &&
			_defaultFontId != INVALID_FONT_VALUE)
		{
			return GetFont(_defaultFontId, _defaultFontSize);
		}
		ENGINE_DEBUG("Default font is not defined yet!");
		return nullptr;
	}

	bool FontController::IsLoading() const
	{
		bool loading = false;

		for (auto& it : _pending_futures)
		{
			loading	|= !utils::isReadyFuture(it);
		}
		
		return loading;
	}

	void FontController::_RegisterFont(uint32_t fontId, const char* fileName)
	{
		const IApp* app = IApp::Get();

		FT_Library library;
		//FT_Face    face;

		if (FT_Init_FreeType(&library))
		{
			ENGINE_FATAL("FT_Init_FreeType failed");
		}

		std::string filePath = app->GetAppConfig().fontsPath + fileName;

		IFileController* fileController = app->GetFileController();
		DataPtr file = fileController->Read(filePath);

		/*if (FT_New_Face(library, filePath.c_str(), 0, &face))
		{
			ENGINE_FATAL("FT_New_Face failed (there is probably a problem with your font file)");
		}*/

		std::vector<std::future<void>> futures;

		for(const auto& fontSize : _fontSizes)
		{
			futures.emplace_back(std::async(&FontController::_AddFont, this, library, file, fontId, fontSize, fileName));
			//_AddFont(library, file, fontId, fontSize, fileName);
		}

		// wait for futures
		std::for_each(futures.cbegin(), futures.cend(), [](const std::future<void>& f) { f.wait(); });

		std::lock_guard<std::mutex> lock(_mutex);

		_fontNamesToIds[fileName] = fontId;

		//FT_Done_Face(face);
		FT_Done_FreeType(library);
	}

	void FontController::_AddFont(FT_Library library, const DataPtr& file, uint32_t fontId, uint32_t fontSize, const char* fileName)
	{
		FT_Face face;

		if (FT_New_Memory_Face(library, (const FT_Byte*)file->GetData(), file->GetSize(), 0, &face))
		{
			ENGINE_FATAL("FT_New_Face failed (there is probably a problem with your font file)");
		}

		FontPtr font = std::make_shared<Font>(face, fontSize, _maxTextureSize, fileName);

		FT_Done_Face(face);

		std::lock_guard<std::mutex> lock(_mutex);

		_registeredFonts[fontSize][fontId] = font;
	}
}