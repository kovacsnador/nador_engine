#ifndef __STB_IMAGE_LOAD_STRATEGY_H__
#define __STB_IMAGE_LOAD_STRATEGY_H__

#include <cstdint>

namespace nador
{
    struct StbImageLoadStrategy
    {
        uint8_t* Load(const uint8_t* buffer, int32_t len, int32_t* width, int32_t* height, int32_t* channels_in_file, int32_t desired_channels) const;
        void     Free(uint8_t* buffer) const;
    };
} // namespace nador

#endif //!__STB_IMAGE_LAOD_STRATEGY_H__