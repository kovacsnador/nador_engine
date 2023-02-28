#ifndef __FILE_UTIL_H__
#define __FILE_UTIL_H__

#include <cstdint>
#include <vector>
#include <string>

#include "Types.h"

namespace am {

	namespace file {

		using path_contener_t		= std::vector<std::string>;
		using segment_container_t	= std::vector<std::string>;
		using img_package_t			= std::vector<path_contener_t>;

		static const std::string NO_SEGMENT = "\\*";
		static const std::string JSON_SEGMENT = "\\*.json";
		static const std::string PNG_SEGMENT = "\\*.png";
		static const std::string JPG_SEGMENT = "\\*.jpg";
		static const std::string BMP_SEGMENT = "\\*.bmp";

		const static int FOLDER = 16;

		class FileUtil {
		public:
			static path_contener_t		SearchFilesInFolder(const std::string& folder_path, const segment_container_t& segment, bool full_path);
			static path_contener_t		SearchSubFolders(const std::string& folder_path, bool full_path);
			static std::string			GetFileNameFromPath(const std::string& full_path);
			static std::string			GetFileName(const std::string& file);
			static img_package_t		SearchPackages(const std::string& root_path, const segment_container_t& segment);
			static void					CreateFolder(const std::string& path);
			static void					SaveFile(const char* path, const byte_t* data, const size_t data_lenght, const char* expansion);
			static segment_container_t	GetImageSegment();
			static int					DeleteFolder(const std::string& folder_path, bool bDeleteSubdirectories = true);

			template<typename ... Args>
			static std::string SF(const std::string& format, Args ... args) {
				int size = snprintf(nullptr, 0, format.c_str(), args...);
				if (size <= 0) {
					return "";
				}
				std::string result(size + 1, '\0');
				snprintf(&result[0], result.size(), format.c_str(), args...);
				return std::move(result);
			}

		private:
			static std::string	_StuckFullPath(const std::string& path, const std::string& file, bool stuck);
			static void			_Search(img_package_t& packages, const std::string& folder_path, const segment_container_t& segment);
			static void			_SearchFilesInFolder(path_contener_t& paths, const std::string& folder_path, const char* segment, bool full_path);

		};

		IMAGE_TYPE DetectImageType(const std::string& file_name);
	}

}

#endif // !__FILE_UTIL_H__




