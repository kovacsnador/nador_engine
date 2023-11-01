#ifndef __WINDOWS_FILE_CONTROLLER_H__
#define __WINDOWS_FILE_CONTROLLER_H__

#include "nador/system/IFileController.h"

namespace nador
{
    class DefaultFileController : public IFileController
    {
    public:
        /*!
         * DefaultFileController constructor.
         *
         * \param data      The root path of the file system.
         */
        DefaultFileController(const std::filesystem::path& rootPath);

        /*!
         * Read a file.
         *
         * \param path      The path of the file.
         *
         * \return The file data.
         */
        [[nodiscard]] std::optional<FileData> Read(const std::filesystem::path& path) const override;

        /*!
         * Writes a file.
         *
         * \param fileName      The name of the file.
         * \param data          The new content of the file.
         * \param size          The size of the content data.
         *
         * \return The true on success, false otherwise.
         */
        bool Write(const std::filesystem::path& path, const char* data, size_t size) const override;

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
         * \param path      The path of the file.
         *
         * \return The true on exist, false otherwise.
         */
        bool IsExist(const std::filesystem::path& path) const override;

        /*!
         * Creates directories
         *
         * \param path      The path of new directories.
         *
         * \return      The true on success, false otherwise.
         */
        bool MakeDirs(const std::filesystem::path& path) const override;

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
