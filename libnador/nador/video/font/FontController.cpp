#include "nador/video/font/FontController.h"
#include "nador/common/ThreadPool.h"
#include "nador/common/Stopwatch.h"

namespace nador
{
    FontController::FontController(const font::FontTextureLoadStrategy_t<Texture>& loadStrategy, uint32_t maxTextureSize)
    : _maxTextureSize(maxTextureSize)
    , _fontTextureLoadStrategy(loadStrategy)
    {
        NADOR_ASSERT(_maxTextureSize > 0);
        NADOR_ASSERT(_fontTextureLoadStrategy);
    }

    bool FontController::AddFontSize(uint32_t fontSize)
    {
        auto [_, inserted] = _fontSizes.insert(fontSize);
        return inserted;
    }

    void FontController::CreateFont(uint32_t fontId, const std::optional<FileData>& file)
    {
        // clear outdated futures
        std::remove_if(_pendingFutures.begin(), _pendingFutures.end(), [this](const auto& it) { return nador::utils::isReadyFuture(it.obj); });

        if (_fontSizes.empty())
        {
            ENGINE_ERROR("No font size registered! Use AddFontSize before!");
            return;
        }

        auto threadPool = GetThreadPool();

        auto future = threadPool->Enqueue(
            [this, fontId, threadPool, file]() {
                if (file.has_value() == false)
                {
                    ENGINE_ERROR("Font could not be ceated with id: %d", fontId);
                    return;
                }

                std::vector<nador::ThreadPoolTask> batchTasks;

                std::vector<nador::MoveableObjWrapper<std::future<void>, nador::FutureWaiter>> futures; // implicit waiting for tasks

                for (auto fontSize : _fontSizes)
                {
                    auto task = nador::CreatePackagedTask([this, fontId, fontSize, file, filePath = file->GetPath().filename()] {
                        // loading the real font
                        _RegisterFont(fontId, fontSize, file.value(), filePath.string(), _maxTextureSize);
                    });
                    futures.emplace_back(task->get_future());
                    batchTasks.emplace_back(std::move(task), nador::ETaskPriority::VERY_HIGH);
                }

                threadPool->Enqueue(batchTasks);
            },
            nador::ETaskPriority::VERY_HIGH);

        _pendingFutures.emplace_back(std::move(future));
    }

    const FontPtr FontController::GetFont(uint32_t fontId, uint32_t fontSize) const
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto fontSizeIter = _registeredFonts.find(fontSize);

        if (fontSizeIter != _registeredFonts.end())
        {
            auto fontIter = fontSizeIter->second.find(fontId);

            if (fontIter != _registeredFonts.at(fontSize).end())
            {
                return fontIter->second;
            }
        }
        ENGINE_WARNING("Font %d not found with size: %d!", fontId, fontSize);
        return nullptr;
    }

    strings_t FontController::GetRegisteredFontNames() const
    {
        std::lock_guard<std::mutex> lock(_mutex);

        strings_t fontNames;
        for (const auto& it : _fontNamesToIds)
        {
            fontNames.push_back(it.first);
        }

        return fontNames;
    }

    uint32_t FontController::GetFontIdByName(const std::string& fontName) const
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto iterator = _fontNamesToIds.find(fontName);
        if (iterator != _fontNamesToIds.end())
        {
            return iterator->second;
        }

        return INVALID_FONT_VALUE;
    }

    strings_t FontController::GetFontSizesAsString() const
    {
        strings_t fontSizesStr;
        for (const auto& it : _fontSizes)
        {
            fontSizesStr.push_back(std::to_string(it));
        }

        return fontSizesStr;
    }

    const FontController::FontSizesList_t& FontController::GetFontSizes() const
    {
        return _fontSizes;
    }

    void FontController::SetDefaultSystemFont(uint32_t fontId, uint32_t fontSize)
    {
        _defaultFontSize = fontSize;
        _defaultFontId   = fontId;
    }

    const FontPtr FontController::GetDefaultSystemFont() const
    {
        if (_defaultFontSize != INVALID_FONT_VALUE && _defaultFontId != INVALID_FONT_VALUE)
        {
            return GetFont(_defaultFontId, _defaultFontSize);
        }
        ENGINE_DEBUG("Default font is not defined yet!");
        return nullptr;
    }

    bool FontController::IsLoading() const
    {
        for (auto& it : _pendingFutures)
        {
            if (utils::isReadyFuture(it.obj) == false)
            {
                return true;
            }
        }
        return false;
    }

    void FontController::_RegisterFont(uint32_t fontId, uint32_t fontSize, FileData file, const std::string& fileName, uint32_t maxTextureSize)
    {
        FT_Library library;

        if (FT_Init_FreeType(&library))
        {
            ENGINE_ERROR("FT_Init_FreeType failed");
            return;
        }

        auto font = _CreateFont(library, file, fontSize, maxTextureSize);

        FT_Done_FreeType(library);

        std::scoped_lock<std::mutex> lock(_mutex);

        _registeredFonts[fontSize][fontId] = std::move(font);
        _fontNamesToIds[fileName]          = fontId;
    }

    void FontController::Wait()
    {
        _pendingFutures.clear();
    }

    FontUPtr FontController::_CreateFont(FT_Library library, const FileData& file, uint32_t fontSize, uint32_t maxTextureSize)
    {
        nador::Stopwatch<std::chrono::system_clock> sw;

        FT_Face face;

        if (FT_New_Memory_Face(library, (const FT_Byte*)file.GetData(), file.GetSize(), 0, &face))
        {
            ENGINE_FATAL("FT_New_Face failed (there is probably a problem with your font file)");
        }

        auto font = std::make_unique<Font>(_fontTextureLoadStrategy, face, fontSize, maxTextureSize);

        FT_Done_Face(face);

        ENGINE_DEBUG("_CreateFont duration: %d ms", sw.Stop<std::chrono::milliseconds>().count());

        return font;
    }
} // namespace nador