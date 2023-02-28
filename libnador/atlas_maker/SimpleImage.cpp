#include "SimpleImage.h"

namespace am {

	namespace sp {

		SimpleImage::SimpleImage(const int width, const int height, const id_t id)
			: _width(width + PIXEL_BORDER)
			, _height(height + PIXEL_BORDER)
			, _id(id)
		{
			//no func
		}

		SimpleImage::SimpleImage(const SimpleImage& copy) {
			_width = copy._width;
			_height = copy._height;
			_id = copy._id;
		}

		SimpleImage::SimpleImage(SimpleImage&& move) {
			_width = move._width;
			_height = move._height;
			_id = move._id;

			move._width = 0;
			move._height = 0;
			move._id = 0;
		}

		SimpleImage& 
		SimpleImage::operator=(const SimpleImage& copy) {
			_width = copy._width;
			_height = copy._height;
			_id = copy._id;
			return *this;
		}

		int
		SimpleImage::GetWidth() const {
			return _width;
		}

		int
		SimpleImage::GetHeight() const {
			return _height;
		}

		int	
		SimpleImage::GetArea() const {
			return (_width * _height);
		}

		id_t
		SimpleImage::GetId() const {
			return _id;
		}

		SimpleImage::~SimpleImage() {
			//no func
		}

	}

}
