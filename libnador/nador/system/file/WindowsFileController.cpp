#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <filesystem>

#include "nador/log/Log.h"
#include "nador/system/file/WindowsFileController.h"
#include "WindowsFileController.h"

namespace nador
{
    WindowsFileController::WindowsFileController(const std::filesystem::path& rootPath)
    : IFileController(rootPath)
    {
    }

    std::optional<FileData> WindowsFileController::Read(const std::filesystem::path& path) const
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
            const auto fileSize = std::filesystem::file_size(path);
            if (fileSize > 0)
            {
                auto data = std::make_shared<char[]>(fileSize);
                file.read(data.get(), fileSize);
                if (file)
                {
                    ENGINE_DEBUG("Read file %s.", path.string().c_str());
                    return FileData{std::move(data), fileSize, path};
                }
                else
                {
                    ENGINE_FATAL("File read error: only %d byte could be read!", file.gcount());
                }
            }
        }
        return std::nullopt;
    }

    bool WindowsFileController::Write(const std::filesystem::path& path, const FileData& data) const
    {
        std::ofstream file(_rootPath / path, std::ofstream::binary);
        if (file.is_open())
        {
            if (data.IsValid())
            {
                file.write(data.GetData(), data.GetSize());
                ENGINE_DEBUG("Write file success! File: %s", path.string().c_str());
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

    bool WindowsFileController::Delete(const char* fileName) const
    {
        return std::filesystem::remove(_rootPath / fileName) == 0;
    }

    bool WindowsFileController::Delete(const std::string& fileName) const
    {
        return Delete(fileName.c_str());
    }

    bool WindowsFileController::IsExist(std::string_view fileName) const
    {
        return std::filesystem::exists(_rootPath / fileName);
    }

    std::string nador::WindowsFileController::GetFileName(std::string_view filePath) const
    {
        return std::filesystem::path(filePath).filename().string();
    }
} // namespace nador