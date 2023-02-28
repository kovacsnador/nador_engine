#ifndef __IMAGE_MANAGER_HPP__
#define __IMAGE_MANAGER_HPP__

#include <vector>
#include <memory>

#include "ImageContainer.hpp"
#include "SimpleImageManager.hpp"
#include "Png.h"
#include "Image.h"

namespace am {

	namespace api {

		class ImageManager {
		public:
			using container_t = std::vector<IImage*>;

			ImageManager() {
				//no func
			}

			ImageManager(const am::file::path_contener_t& paths) {
				if (paths.size()) {
					_folder_path = paths.at(0);
					for (size_t i = 1; i < paths.size(); ++i) {
						CreateImage(paths.at(i));
					}
				}
			}

			void 
			CreateImage(const std::string& file_path) {
				_container.push_back(am::Image::CreateImage(file_path));
			}

			std::vector<id_t>
			GetAllId() const {
				std::vector<id_t> temp;
				for (const auto& it : _container) {
					temp.push_back(it->GetId());
				}
				return temp;
			}

			sp::SimpleImageManager<>
			GetSimpleClone() const {
				sp::SimpleImageManager<> temp;
				for (const auto& it : _container) {
					temp.PushBack(it->GetSimpleImageClone());
				}
				return temp;
			}

			const std::string&
			GetFolderPath()const {
				return _folder_path;
			}

		private:
			container_t _container;
			std::string _folder_path;
		};

	}

}

#endif //!__IMAGE_MANAGER_HPP__
