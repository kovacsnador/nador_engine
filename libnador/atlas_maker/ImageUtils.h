#ifndef __IMAGE_UTILS_H__
#define __IMAGE_UTILS_H__

#include "Vec2d.hpp"
#include "SimpleAtlas.hpp"

namespace am {

	namespace utils {

		struct UVCordinates {
			math::Vec2d<float> _uv_s[4];
		};

		UVCordinates CalculateUVS(const sp::ImageInSAtlas& image, const int atlas_w, const int atlas_h);

	}

}



#endif __IMAGE_UTILS_H__ //!__IMAGE_UTILS_H__
