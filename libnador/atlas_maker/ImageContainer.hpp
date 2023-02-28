#ifndef __IMAGE_CONTAINER_HPP__
#define __IMAGE_CONTAINER_HPP__

#include <map>
#include <mutex>
#include <vector>

#include "FileUtil.h"

#define IMAGE_CONTAINER am::api::ImageContainer<>::GetInstance()

namespace am {

	struct ImageDataEasy {
		std::string	_file_path;
	};

	using id_t = size_t;
	using byte_t = unsigned char;

	class IImage;

	namespace api {

		//singleton class
		template<typename T1 = IImage*>
		class ImageContainer {
		
		public:
			using key_type = id_t;
			using value_type = T1;
			using container_t = std::map<key_type, value_type>;

			//--------------delete--------------
			ImageContainer(const ImageContainer& copy) = delete;
			ImageContainer& operator=(const ImageContainer& copy) = delete;
			//----------------------------------

			static ImageContainer&
			GetInstance() {
				static ImageContainer<value_type> instance;
				return instance;
			}

			value_type
			GetById(const id_t id) {
				std::lock_guard<std::mutex> lock(_image_container_mutex);
				auto it = _storage.find(id);
				if (it == _storage.end()) {
					throw std::runtime_error(file::FileUtil::SF("Image id : %d doesnt exist!", id));
				}
				return it->second;
			}

			bool 
			IsEmpty() {
				std::lock_guard<std::mutex> lock(_image_container_mutex);
				return _storage.empty();
			}

			void 
			Clear() {
				std::lock_guard<std::mutex> lock(_image_container_mutex);
				_storage.clear();
			}

			key_type 
			PushBack(const value_type& data) {
				std::lock_guard<std::mutex> lock(_image_container_mutex);
				key_type key;
				key = _storage.size();
				_storage.insert(std::pair<key_type, value_type>(key, data));
				return key;
			}

			void
			Erase(const key_type& key) {
				std::lock_guard<std::mutex> lock(_image_container_mutex);
				auto it = _storage.find(key);
				if (it == _storage.end()) {
					throw std::runtime_error(file::FileUtil::SF("Image id : %d doesnt exist!", key));
				}
				delete it->second;
				_storage.erase(key);
			}

			key_type
			GetSize() {
				std::lock_guard<std::mutex> lock(_image_container_mutex);
				return _storage.size();
			}

		private:
			ImageContainer() { };

			~ImageContainer() {
				std::lock_guard<std::mutex> lock(_image_container_mutex);
				for (auto& it : _storage) {
					delete it.second;
				}
			};

			std::mutex				_image_container_mutex;
			container_t				_storage;
		};

	}

}


#endif //!__IMAGE_CONTAINER_HPP__
