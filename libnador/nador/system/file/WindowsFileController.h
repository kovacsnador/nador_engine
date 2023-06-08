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
        WindowsFileController(const std::string_view& rootPath);

        /*!
         * Read a file.
         *
         * \param fileName      The name of the file.
         *
         * \return The file data.
         */
        FileDataPtr Read(const char* fileName) const override;

        /*!
         * Read a file.
         *
         * \param fileName      The name of the file.
         *
         * \return The file data.
         */
        FileDataPtr Read(const std::string& fileName) const override;

        /*!
         * Writes a file.
         *
         * \param fileName      The name of the file.
         * \param data          The new content of the file.
         *
         * \return The true on success, false otherwise.
         */
        bool Write(const char* fileName, const FileDataPtr& data) const override;

        /*!
         * Writes a file.
         *
         * \param fileName      The name of the file.
         * \param data          The new content of the file.
         *
         * \return The true on success, false otherwise.
         */
        bool Write(const std::string& fileName, const FileDataPtr& data) const override;

        /*!
         * Deletes a file.
         *
         * \param fileName      The name of the file.
         *
         * \return The true on success, false otherwise.
         */
        bool Delete(const char* fileName) const override;

        /*!
         * Deletes a file.
         *
         * \param fileName      The name of the file.
         *
         * \return The true on success, false otherwise.
         */
        bool Delete(const std::string& fileName) const override;

        /*!
         * Checks if the file exist.
         *
         * \param fileName      The name of the file.
         *
         * \return The true on exist, false otherwise.
         */
        bool IsExist(const char* fileName) const override;

        /*!
         * Checks if the file exist.
         *
         * \param fileName      The name of the file.
         *
         * \return The true on exist, false otherwise.
         */
        bool IsExist(const std::string& fileName) const override;

        std::string GetFileName(std::string_view filePath) const override;
	};
}

#endif // !__WINDOWS_FILE_CONTROLLER_H__

