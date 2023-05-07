#include "stb_image/stb_image.h"

#include "nador/video/StbImageLoadStrategy.h"

namespace nador
{
    uint8_t* StbImageLoadStrategy::Load(const uint8_t* buffer,
                                        int32_t        len,
                                        int32_t*       width,
                                        int32_t*       height,
                                        int32_t*       channels_in_file,
                                        int32_t        desired_channels) const
    {
        stbi_set_flip_vertically_on_load(1);
		return stbi_load_from_memory((stbi_uc*)buffer, len, width, height, channels_in_file, desired_channels);
    }

    void StbImageLoadStrategy::Free(uint8_t* buffer) const
    {
        if(buffer)
		{
			stbi_image_free(buffer);
		}
    }
} // namespace nador