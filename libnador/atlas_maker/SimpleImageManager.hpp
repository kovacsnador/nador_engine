#ifndef __SIMPLE_IMAGE_MANAGER_HPP__
#define __SIMPLE_IMAGE_MANAGER_HPP__

#include <algorithm>
#include <functional>

#include "SimpleImage.h"

namespace am {

	namespace sp {

		enum SORT_TYPE {
			BY_HEIGHT,
			BY_WIDTH,
			BY_AREA
		};

		template<class T = SimpleImage>
		class SimpleImageManager {
		public:

			using container_t = std::vector<T>;

			SimpleImageManager() {
				//no func
			}

			SimpleImageManager(const SimpleImageManager<T>& copy) {
				_container = copy._container;
			}

			SimpleImageManager<T>&
			operator=(const SimpleImageManager<T>& copy) {
				_container = copy._container;
			}

			void 
			PushBack(const T& img) {
				_container.push_back(img);
			}

			void
			Sort(const SORT_TYPE type = BY_AREA) {
				std::function<bool(const T&, const T&)> compare_func;
				switch (type) {
				case BY_HEIGHT:
					compare_func = [](const T& i, const T& j)->bool {
						return (i.GetHeight() > j.GetHeight());
					};
					break;
				case BY_WIDTH:
					compare_func = [](const T& i, const T& j)->bool {
						return (i.GetWidth() > j.GetWidth());
					};
					break;
				case BY_AREA:
					compare_func = [](const T& i, const T& j)->bool {
						return (i.GetArea() > j.GetArea());
					};
					break;
				default:
					compare_func = [](const T& i, const T& j)->bool {
						return (i.GetArea() > j.GetArea());
					};
					break;
				}
				std::sort(_container.begin(), _container.end(), compare_func);
			}

			const container_t&
			GetContainer() const {
				return _container;
			}

		private:
			container_t _container;
		};

	}

}


#endif //!__SIMPLE_IMAGE_MANAGER_HPP__
