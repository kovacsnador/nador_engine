#ifndef __SIMPLE_IMAGE_H__
#define __SIMPLE_IMAGE_H__

#include <string>

#include "ImageContainer.hpp"

namespace am {

	namespace sp {

		class SimpleImage {
		public:
			static const int PIXEL_BORDER = 1;

			SimpleImage(const int width, const int height, const id_t id);
			SimpleImage(const SimpleImage& copy);
			SimpleImage(SimpleImage&& move);

			SimpleImage& operator=(const SimpleImage& copy);


			int		GetWidth() const;
			int		GetHeight() const;
			int		GetArea() const;
			id_t	GetId() const;

			virtual ~SimpleImage();

		private:
			int		_width;
			int		_height;
			id_t	_id;
		};

	}

}

#endif //!__SIMPLE_IMAGE_H__

