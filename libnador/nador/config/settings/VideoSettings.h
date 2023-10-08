#ifndef __NADOR_VIDEO_SETTINGS_H__
#define __NADOR_VIDEO_SETTINGS_H__

#include <cstdint>

namespace nador
{
    struct VideoSettings
    {
        size_t maxVertexCount;

        bool operator==(const VideoSettings& other) const = default;
    };
} // namespace nador


#endif //!__NADOR_VIDEO_SETTINGS_H__