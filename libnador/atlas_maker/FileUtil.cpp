#include <Windows.h>
#include <cstdio>
#include <string>
#include <fstream>
#include <cassert>
#include <filesystem>
#include <iostream>

#include "FileUtil.h"

namespace am {

	namespace file {

		path_contener_t
		FileUtil::SearchFilesInFolder(const std::string& folder_path, const segment_container_t& segment, bool full_path) {
			path_contener_t storage;
			storage.push_back(folder_path);
			for (const auto& seg : segment) {
				_SearchFilesInFolder(storage, folder_path, seg.c_str(), full_path);
			}
			return std::move(storage);
		}

		void
		FileUtil::_SearchFilesInFolder(path_contener_t& paths, const std::string& folder_path, const char* segment, bool full_path) {
			assert(segment);
			WIN32_FIND_DATA file;
			std::string path_temp = folder_path + segment;
			HANDLE search_handle = FindFirstFile(path_temp.c_str(), &file);
			if (search_handle && search_handle != INVALID_HANDLE_VALUE) {
				do {
					paths.push_back(_StuckFullPath(folder_path, file.cFileName, full_path));
				} while (FindNextFile(search_handle, &file));
				FindClose(search_handle);
			}
		}

		path_contener_t
		FileUtil::SearchSubFolders(const std::string& folder_path, bool full_path) {
			path_contener_t storage;
			WIN32_FIND_DATA file;
			std::string path_temp = folder_path + NO_SEGMENT;
			HANDLE search_handle = FindFirstFile(path_temp.c_str(), &file);
			if (search_handle && search_handle != INVALID_HANDLE_VALUE) {
				do {
					if (file.dwFileAttributes == FOLDER && strcmp(file.cFileName, ".") && strcmp(file.cFileName, "..") && strcmp(ATLAS_FOLDER_NAME.c_str(), file.cFileName)) {
						storage.push_back(_StuckFullPath(folder_path, file.cFileName, full_path));
					}
				} while (FindNextFile(search_handle, &file));
				FindClose(search_handle);
			}
			return std::move(storage);
		}

		std::string
		FileUtil::_StuckFullPath(const std::string& path, const std::string& file, bool stuck) {
			if (stuck) {
				return path + "\\" + file;
			}
			return file;
		}

		segment_container_t
		FileUtil::GetImageSegment() {
			segment_container_t temp;
			temp.push_back(PNG_SEGMENT);
			temp.push_back(JPG_SEGMENT);
			temp.push_back(BMP_SEGMENT);
			return std::move(temp);
		}

		std::string
		FileUtil::GetFileNameFromPath(const std::string& full_path) {
			size_t first = full_path.find_last_of("/\\");
			size_t last = full_path.find_last_of(".");
			return full_path.substr(first + 1, last - first - 1);
		}

		std::string
		FileUtil::GetFileName(const std::string& file) {
			size_t f = file.find_last_of("/\\");
			int first = f;
			if(f == std::string::npos) {
				first = -1;
			}
			size_t last = file.find_last_of(".");
			return file.substr(first + 1, last - first - 1);
		}

		void
		FileUtil::SaveFile(const char* path, const byte_t* data, const size_t data_lenght, const char* expansion) {
			std::ofstream file(std::string(path) + expansion, std::ios::out | std::ios::binary);
			if (file.is_open()) {
				file.write((char*)data, data_lenght);
			}
			file.close();
		}

		img_package_t
		FileUtil::SearchPackages(const std::string& root_path, const segment_container_t& segment) {
			std::cout << "searching packeges..." << std::endl;
			img_package_t temp;
			_Search(temp, root_path, segment);
			std::cout << "found packeges: " << temp.size() << std::endl;
			return std::move(temp);
		}

		void
		FileUtil::CreateFolder(const std::string& path) {
			CreateDirectory(path.c_str(), NULL);
		}

		int
		FileUtil::DeleteFolder(const std::string& folder_path, bool bDeleteSubdirectories) {

			bool            b_subdirectory = false;      // Flag, indicating whether
														 // subdirectories have been found
			HANDLE          h_file;                      // Handle to directory
			std::string     str_file_path;               // Filepath
			std::string     str_pattern;                  // Pattern
			WIN32_FIND_DATA file_information;             // File information


			str_pattern = folder_path + "\\*.*";
			h_file = ::FindFirstFile(str_pattern.c_str(), &file_information);
			if (h_file != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (file_information.cFileName[0] != '.')
					{
						str_file_path.erase();
						str_file_path = folder_path + "\\" + file_information.cFileName;

						if (file_information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							if (bDeleteSubdirectories)
							{
								// Delete subdirectory
								int iRC = DeleteFolder(str_file_path, bDeleteSubdirectories);
								if (iRC)
									return iRC;
							}
							else
								b_subdirectory = true;
						}
						else
						{
							// Set file attributes
							if (::SetFileAttributes(str_file_path.c_str(),
								FILE_ATTRIBUTE_NORMAL) == FALSE)
								return ::GetLastError();

							// Delete file
							if (::DeleteFile(str_file_path.c_str()) == FALSE)
								return ::GetLastError();
						}
					}
				} while (::FindNextFile(h_file, &file_information) == TRUE);

				// Close handle
				::FindClose(h_file);

				DWORD dwError = ::GetLastError();
				if (dwError != ERROR_NO_MORE_FILES)
					return dwError;
				else
				{
					if (!b_subdirectory)
					{
						// Set directory attributes
						if (::SetFileAttributes(folder_path.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE) {
							return ::GetLastError();
						}

						// Delete directory
						if (::RemoveDirectory(folder_path.c_str()) == FALSE)
							return ::GetLastError();
					}
				}
			}
			return 0;
		}

		void
		FileUtil::_Search(img_package_t& packages, const std::string& folder_path, const segment_container_t& segment) {
			path_contener_t files = SearchFilesInFolder(folder_path, segment, true);
			if (!files.empty()) {
				packages.push_back(files);
			}
			path_contener_t sub_folders = SearchSubFolders(folder_path, true);
			if (!sub_folders.empty()) {
				for (const auto& it : sub_folders) {
					_Search(packages, it, segment);
				}
			}
		}

		IMAGE_TYPE
		DetectImageType(const std::string& file_name) {
			size_t res = file_name.find(PNG_PREFIX);
			if (res != std::string::npos) {
				return IMAGE_TYPE::PNG;
			}
			res = file_name.find(JPEG_PREFIX);
			if (res != std::string::npos) {
				return IMAGE_TYPE::JPEG;
			}
			res = file_name.find(BMP_PREFIX);
			if (res != std::string::npos) {
				return IMAGE_TYPE::BMP;
			}
			return IMAGE_TYPE::NOT_IMAGE;
		}

	}
}
