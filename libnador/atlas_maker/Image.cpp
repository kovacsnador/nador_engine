#include <iostream>

#include "Image.h"
#include "FileUtil.h"

namespace am {

	Image* Image::CreateImage(const std::string& file_path) {
		return new Image(file_path);
	}

	Image::Image(const std::string& file_path)
		: _image(file_path.c_str())
		, _file_path(file_path)
	{
		std::cout << "Read image : " << file_path << std::endl;
	}

	Image::Image(const int width, const int height)
		: _image(width, height)
	{
		//no func
	}

	Image::Image(const int width, const int height, const int depth, const int channels)
		: _image(width, height, depth, channels, 0)
	{
		//no func
	}

	int
	Image::GetWidth() const {
		return _image.width();
	}

	int	
	Image::GetHeight() const {
		return _image.height();
	}

	const byte_t*
	Image::GetRow(const int row) const {
		return _image.data() + (row * GetWidth());
	}

	const byte_t*
	Image::GetData() const {
		return _image.data();
	}

	byte_t* 
	Image::GetData() {
		return _image.data();
	}

	int	
	Image::GetDataSize() const {
		return _image.size();
	}

	sp::SimpleImage
	Image::GetSimpleImageClone() const {
		return sp::SimpleImage(_image.width(), _image.height(), _id);
	}

	std::string 
	Image::GetName() const {
		return std::move(file::FileUtil::GetFileNameFromPath(_file_path));
	}

	int	
	Image::SetSprectrum(const int channel) {
		_image.channels(0, channel);
		return _image.spectrum();
	}

	int	
	Image::GetSprectrum() const {
		return _image.spectrum();
	}

	Image::~Image() {
		//no func
	}


}