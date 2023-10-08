#ifndef __NADOR_FILE_DATA_H__
#define __NADOR_FILE_DATA_H__

#include <memory>
#include <cstdint>
#include <filesystem>

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
} // namespace nador

#endif //!__NADOR_FILE_DATA_H__