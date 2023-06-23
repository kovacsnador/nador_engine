#ifndef __WINDOWS_FILE_CONTROLLER_H__
#define __WINDOWS_FILE_CONTROLLER_H__

#include "nador/system/IFileController.h"

namespace nador
{
    class WindowsFileController : public IFileController
    {
    public:
        /*!
         * WindowsFileController constructor.
         *
         * \param data      The root path of the file system.
         */
        WindowsFileController(const std::filesystem::path& rootPath);

        /*!
         * Read a file.
         *
         * \param path      The path of the file.
         *
         * \return The file data.
         */
        std::optional<FileData> Read(const std::filesystem::path& path) const override;

        /*!
         * Writes a file.
         *
         * \param fileName      The name of the file.
         * \param data          The new content of the file.
         *
         * \return The true on success, false otherwise.
         */
        bool Write(const std::filesystem::path& path, const FileData& data) const override;

        /*!
         * Deletes a file.
         *
         * \param fileName      The name of the file.
         *
         * \return The true on success, false otherwise.
         */
        bool Delete(const std::filesystem::path& path) const override;

        /*!
         * Checks if the file exist.
         *
         * \param fileName      The name of the file.
         *
         * \return The true on exist, false otherwise.
         */
        bool IsExist(std::string_view fileName) const override;

        /*!
         * Get the file name if it's a regular file from the path.
         *
         * \param filePath      The file path.
         *
         * \return      The name of the file.
         */
        std::string GetFileName(std::string_view filePath) const override;
    };
} // namespace nador

#endif // !__WINDOWS_FILE_CONTROLLER_H__
