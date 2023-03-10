#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "nador/log/Log.h"
#include "nador/system/file/WindowsFileController.h"

namespace nador
{
	WindowsFileController::WindowsFileController(const std::string_view& rootPath)
	: IFileController(rootPath)
	{
	}

	DataPtr WindowsFileController::Read(const char* fileName) const
	{
        NADOR_ASSERT(fileName);
        std::ifstream file(_rootPath + fileName, std::ifstream::binary);
        if(file.is_open())
        {
            file.seekg(0, file.end);
            int64_t size = file.tellg();
            file.seekg(0, file.beg);
            if(size > 0)
            {
                char* const data = new char[(uint32_t)size];
                file.read(data, size);
                if(file)
                {
                    file.close();
                    ENGINE_DEBUG("Read file %s.", fileName);
                    return std::make_shared<Data>(data, (size_t)size);
                }
                else
                {
                    file.close();
                    ENGINE_FATAL("File read error: only %d byte could be read!", file.gcount());
                }
            }
            file.close();
        }
        else
        {
            ENGINE_WARNING("Could not open (maybe not exist) file: %s", fileName);
        }
        return std::make_shared<Data>(nullptr, 0);
	}

    DataPtr WindowsFileController::Read(const std::string& fileName) const
    {
        return Read(fileName.c_str());
    }

	bool WindowsFileController::Write(const char* fileName, const DataPtr& data) const
	{
        NADOR_ASSERT(fileName);
        std::ofstream file(_rootPath + fileName, std::ofstream::binary);
        if(file.is_open())
        {
            if(data->IsValid())
            {
                file.write(data->GetData(), data->GetSize());
                ENGINE_DEBUG("Write file success! File: %s", fileName);
                file.close();
                return true;
            }
            else
            {
                file.close();
                ENGINE_FATAL("You try to write a file with nullptr, or a 0 size of data! File: %s", fileName);
            }
        }
        ENGINE_WARNING("Could not open file: %s", fileName);
        return false;
	}

    bool WindowsFileController::Write(const std::string& fileName, const DataPtr& data) const
    {
        return Write(fileName.c_str(), data);
    }

	bool WindowsFileController::Delete(const char* fileName) const
	{
        return std::remove((_rootPath + fileName).c_str()) == 0;
	}

    bool WindowsFileController::Delete(const std::string& fileName) const
    {
        return Delete(fileName.c_str());
    }

	bool WindowsFileController::IsExist(const char* fileName) const
	{
        struct stat buffer;
        return (stat((_rootPath + fileName).c_str(), &buffer) == 0);
	}

    bool WindowsFileController::IsExist(const std::string& fileName) const
    {
        return IsExist(fileName.c_str());
    }
}