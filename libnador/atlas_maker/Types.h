#ifndef __TYPES_H__
#define __TYPES_H__

#define FLOAT_CAST static_cast<float>

namespace am {

	static const char* const PNG_PREFIX = ".png";
	static const char* const JPEG_PREFIX = ".jpeg";
	static const char* const BMP_PREFIX = ".bmp";

	static const std::string ATLAS_FOLDER_NAME = "__atlases";

	using byte_t = unsigned char;

	enum class IMAGE_TYPE {
		PNG,
		JPEG,
		BMP,
		NOT_IMAGE
	};

}

#endif //!__TYPES_H__
