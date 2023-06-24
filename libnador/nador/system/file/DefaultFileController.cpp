#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <filesystem>

#include "nador/log/Log.h"
#include "nador/system/file/DefaultFileController.h"

namespace nador
{
    DefaultFileController::DefaultFileController(const std::filesystem::path& rootPath)
    : IFileController(rootPath)
    {
    }

    std::optional<FileData> DefaultFileController::Read(const std::filesystem::path& path) const
    {
        auto p = _rootPath / path;

        if (std::filesystem::exists(p) == false)
        {
            NADOR_WARNING("File not exist: %s", p.string().c_str());
            return std::nullopt;
        }

        if (std::filesystem::is_regular_file(p) == false)
        {
            NADOR_WARNING("File is not a regular file: %s", p.string().c_str());
            return std::nullopt;
        }

        std::ifstream file(p, std::ifstream::binary);
        if (file.is_open())
        {
            const auto fileSize = std::filesystem::file_size(p);
            if (fileSize > 0)
            {
                auto data = std::make_shared<char[]>(fileSize);
                file.read(data.get(), fileSize);
                if (file)
                {
                    ENGINE_DEBUG("Read file %s.", path.string().c_str());
                    return FileData { std::move(data), fileSize, path };
                }
                else
                {
                    ENGINE_FATAL("File read error: only %d byte could be read!", file.gcount());
                }
            }
        }
        return std::nullopt;
    }

    bool DefaultFileController::Write(const std::filesystem::path& path, const char* data, size_t size) const
    {
        std::ofstream file(_rootPath / path, std::ofstream::binary);
        if (file.is_open())
        {
            if (data)
            {
                file.write(data, size);
                ENGINE_DEBUG("Write file success with %d bytes. File: %s", size, path.string().c_str());
                return true;
            }
            else
            {
                ENGINE_FATAL("You try to write a file with nullptr, or a 0 size of data! File: %s", path.string().c_str());
            }
        }
        ENGINE_WARNING("Could not open file: %s", path.string().c_str());
        return false;
    }

    bool DefaultFileController::Delete(const std::filesystem::path& path) const
    {
        return std::filesystem::remove_all(_rootPath / path);
    }

    bool DefaultFileController::IsExist(const std::filesystem::path& path) const
    {
        return std::filesystem::exists(_rootPath / path);
    }

    bool DefaultFileController::MakeDirs(const std::filesystem::path& path) const
    {
        return std::filesystem::create_directories(path);
    }

    std::string nador::DefaultFileController::GetFileName(std::string_view filePath) const
    {
        return std::filesystem::path(filePath).filename().string();
    }
} // namespace nador