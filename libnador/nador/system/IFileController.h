#ifndef __IFILE_CONTROLLER_H__
#define __IFILE_CONTROLLER_H__

#include <functional>
#include <memory>
#include <string>

#include "nador/utils/Types.h"

namespace nador
{
    struct FileData
    {
        using data_t = char;
        using deleter_t = std::function<void(const data_t*)>;

        /*!
         * FileData constructor.
         *
         * \param data      The raw data.
         * \param size      The size if the data.
         * \param deleter   The deleter if the data.
         */
        FileData(const data_t* data, size_t size, std::string_view path, deleter_t deleter = [](const data_t* data) { delete[] data; })
        : _data(data)
        , _size(size)
        , _deleter(deleter)
        , _path(path)
        {
        }

        /*!
         * FileData destructor. 
         */
        ~FileData()
        {
            if(_data)
            {
                _deleter(_data);
            }
        }

        /*!
         * Gets the data.
         *
         * \return      The raw data pointer.
         */
        const data_t* GetData() const
        { 
            return _data;
        }

        /*!
         * Gets the begin of the data.
         *
         * \return      The raw data pointer.
         */
        const data_t* Begin() const
        {
            return _data;
        }

        /*!
         * Gets the end of the data.
         *
         * \return      The raw data pointer.
         */
        const data_t* End() const
        {
            return _data + _size;
        }

        /*!
         * Gets the data size.
         *
         * \return      The size of the data.
         */
        size_t GetSize() const
        {
            return _size;
        }

        /*!
         * Check if the File is valid or not.
         *
         * \return      True if valid, false otherwise.
         */
        bool IsValid() const
        {
            return (_data && _size);
        }

        /*!
         * Gets the original file path.
         *
         * \return      The file path.
         */
        const std::string& GetPath() const
        {
            return _path;
        }

    private:
        const data_t*   _data;
        const size_t    _size;
        deleter_t       _deleter;
        std::string     _path;
    };

    CREATE_PTR_TYPES(FileData);


	class IFileController
	{
    public:
        /*!
         * IFileController constructor.
         *
         * \param data      The root path of the file system.
         */
        IFileController(const std::string_view& rootPath)
        : _rootPath(rootPath)
        {
        }

        /*!
         * IFileController destructor.
         */
        virtual ~IFileController() = default;

        /*!
         * Read a file.
         *
         * \param fileName      The name of the file.
         *
         * \return The file data.
         */
        virtual FileDataPtr Read(const char* fileName) const = 0;

        /*!
         * Read a file.
         *
         * \param fileName      The name of the file.
         *
         * \return The file data.
         */
        virtual FileDataPtr Read(const std::string& fileName) const = 0;
        
        /*!
         * Writes a file.
         *
         * \param fileName      The name of the file.
         * \param data          The new content of the file.
         *
         * \return The true on success, false otherwise.
         */
        virtual bool Write(const char* fileName, const FileDataPtr& data) const = 0;

        /*!
         * Writes a file.
         *
         * \param fileName      The name of the file.
         * \param data          The new content of the file.
         *
         * \return The true on success, false otherwise.
         */
        virtual bool Write(const std::string& fileName, const FileDataPtr& data) const = 0;

        /*!
         * Deletes a file.
         *
         * \param fileName      The name of the file.
         *
         * \return The true on success, false otherwise.
         */
        virtual bool Delete(const char* fileName) const = 0;

        /*!
         * Deletes a file.
         *
         * \param fileName      The name of the file.
         *
         * \return The true on success, false otherwise.
         */
        virtual bool Delete(const std::string& fileName) const = 0;
        
        /*!
         * Checks if the file exist.
         *
         * \param fileName      The name of the file.
         *
         * \return The true on exist, false otherwise.
         */
        virtual bool IsExist(const char* fileName) const = 0;
        
        /*!
         * Checks if the file exist.
         *
         * \param fileName      The name of the file.
         *
         * \return The true on exist, false otherwise.
         */
        virtual bool IsExist(const std::string& fileName) const = 0;

        /*!
         * Gets the root path from the file controller.
         *
         * \return      The root file path.
         */
        virtual const std::string& GetRootPath() const
        {
            return _rootPath;
        }

        /*!
         * Sets the root path for the file controller.
         *
         * \param root      The new root file path.
         */
        virtual void SetRootPath(const std::string& root)
        {
            _rootPath = root;
        }

        virtual std::string GetFileName(std::string_view filePath) const = 0;
    
    protected:
        std::string _rootPath;
	};
    CREATE_PTR_TYPES(IFileController);
}

#endif // !__IFILE_CONTROLLER_H__