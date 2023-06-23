#ifndef __IFILE_CONTROLLER_H__
#define __IFILE_CONTROLLER_H__

#include <functional>
#include <memory>
#include <string>
#include <filesystem>
#include <optional>

#include "nador/utils/Types.h"
#include "nador/utils/NonCopyable.h"

namespace nador
{
    struct FileData
    {
        using data_t = char;

        /*!
         * FileData constructor.
         *
         * \param data      The raw data.
         * \param size      The size if the data.
         * \param deleter   The deleter if the data.
         */
        FileData(std::shared_ptr<data_t[]> data, size_t size, const std::filesystem::path& path)
        : _data(std::move(data))
        , _size(size)
        , _path(path)
        {
        }

        /*!
         * Gets the data.
         *
         * \return      The raw data pointer.
         */
        const data_t* GetData() const { return _data.get(); }

        /*!
         * Gets the begin of the data.
         *
         * \return      The raw data pointer.
         */
        const data_t* Begin() const { return _data.get(); }

        /*!
         * Gets the end of the data.
         *
         * \return      The raw data pointer.
         */
        const data_t* End() const { return _data.get() + _size; }

        /*!
         * Gets the data size.
         *
         * \return      The size of the data.
         */
        size_t GetSize() const { return _size; }

        /*!
         * Check if the File is valid or not.
         *
         * \return      True if valid, false otherwise.
         */
        bool IsValid() const { return (_data && _size); }

        /*!
         * Gets the original file path.
         *
         * \return      The file path.
         */
        const std::filesystem::path& GetPath() const { return _path; }

    private:
        std::shared_ptr<data_t[]> _data;
        const size_t              _size;
        std::filesystem::path     _path;
    };

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
         *
         * \return The true on success, false otherwise.
         */
        virtual bool Write(const std::filesystem::path& path, const FileData& data) const = 0;

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
         * \param fileName      The name of the file.
         *
         * \return The true on exist, false otherwise.
         */
        virtual bool IsExist(std::string_view fileName) const = 0;

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