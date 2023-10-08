#ifndef __IFILE_CONTROLLER_H__
#define __IFILE_CONTROLLER_H__

#include <functional>
#include <memory>
#include <string>
#include <filesystem>
#include <optional>

#include "nador/utils/Types.h"
#include "nador/utils/NonCopyable.h"
#include "nador/system/file/FileData.h"

namespace nador
{
    class IFileController
    {
    public:
        /*!
         * IFileController constructor.
         *
         * \param data      The root path of the file system.
         */
        IFileController(std::filesystem::path rootPath)
        : _rootPath(std::move(rootPath))
        {
        }

        /*!
         * IFileController destructor.
         */
        virtual ~IFileController() = default;

        /*!
         * Read a file.
         *
         * \param path      The path of the file.
         *
         * \return The file data.
         */
        virtual std::optional<FileData> Read(const std::filesystem::path& path) const = 0;

        /*!
         * Writes a file.
         *
         * \param fileName      The name of the file.
         * \param data          The new content of the file.
         * \param size          The size of the content data.
         *
         * \return The true on success, false otherwise.
         */
        virtual bool Write(const std::filesystem::path& path, const char* data, size_t size) const = 0;

        /*!
         * Deletes a file.
         *
         * \param path      The name of the file.
         *
         * \return The true on success, false otherwise.
         */
        virtual bool Delete(const std::filesystem::path& path) const = 0;

        /*!
         * Checks if the file exist.
         *
         * \param path      The path of the file.
         *
         * \return The true on exist, false otherwise.
         */
        virtual bool IsExist(const std::filesystem::path& path) const = 0;

        /*!
         * Creates directories
         *
         * \param path      The path of new directories.
         *
         * \return      The true on success, false otherwise.
         */
        virtual bool MakeDirs(const std::filesystem::path& path) const = 0;

        /*!
         * Get the file name if it's a regular file from the path.
         *
         * \param filePath      The file path.
         * 
         * \return      The name of the file.
         */
        virtual std::string GetFileName(std::string_view filePath) const = 0;

        /*!
         * Gets the root path from the file controller.
         *
         * \return      The root file path.
         */
        virtual const std::filesystem::path& GetRootPath() const { return _rootPath; }

        /*!
         * Sets the root path for the file controller.
         *
         * \param root      The new root file path.
         */
        virtual void SetRootPath(const std::filesystem::path& root) { _rootPath = root; }

    protected:
        std::filesystem::path _rootPath;
    };
    CREATE_PTR_TYPES(IFileController);
} // namespace nador

#endif // !__IFILE_CONTROLLER_H__