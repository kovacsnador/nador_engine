#ifndef __NADOR_VIDEO_SETTINGS_H__
#define __NADOR_VIDEO_SETTINGS_H__

#include <cstdint>

#include "nador/utils/TinyXmlHelper.h"

namespace nador
{
    struct VideoSettings
    {
        size_t maxVertexCount;

        bool operator==(const VideoSettings& other) const
        {
            return maxVertexCount == other.maxVertexCount;
        }
    };

    inline void Parse(const tinyxml2::XMLElement* elem, VideoSettings& settings)
    {
        settings.maxVertexCount = xml::GetByName<uint32_t>(elem, "MaxVertexCount");
    }

} // namespace nador


#endif //!__NADOR_VIDEO_SETTINGS_H__