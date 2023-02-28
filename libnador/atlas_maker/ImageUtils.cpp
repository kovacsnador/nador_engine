#include "ImageUtils.h"
#include "Types.h"

namespace am {

	namespace utils {

		/**
		 * Calcualte the uv coordinates from the image(this is for PNG)
		 */
		UVCordinates
		CalculateUVS(const sp::ImageInSAtlas& image, const int atlas_w, const int atlas_h) {
			UVCordinates uvs;

			math::Vec2d<int> pos = image.GetPosition();

			uvs._uv_s[0].SetX(FLOAT_CAST(pos.X()) / atlas_w);						//left down
			uvs._uv_s[0].SetY(1 - (FLOAT_CAST(pos.Y() + image.GetHeight()) / atlas_h));

			uvs._uv_s[1].SetX(FLOAT_CAST(pos.X() + image.GetWidth()) / atlas_w);	//right down
			uvs._uv_s[1].SetY(1 - (FLOAT_CAST(pos.Y() + image.GetHeight()) / atlas_h));

			uvs._uv_s[2].SetX(FLOAT_CAST(pos.X() + image.GetWidth()) / atlas_w);	//right top
			uvs._uv_s[2].SetY(1 - (FLOAT_CAST(pos.Y()) / atlas_h));

			uvs._uv_s[3].SetX(FLOAT_CAST(pos.X()) / atlas_w);						//left top
			uvs._uv_s[3].SetY(1 - (FLOAT_CAST(pos.Y()) / atlas_h));

			return uvs;
		}

	}

}