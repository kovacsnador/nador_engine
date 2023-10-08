#ifndef __NADOR_ATLAS_SETTINGS_H__
#define __NADOR_ATLAS_SETTINGS_H__

#include <filesystem>

namespace nador
{
    struct AtlasSettings
    {
        std::filesystem::path atlasConfigPath;
        std::filesystem::path atlasImagesPath;
        size_t atlasCacheSize;

        bool operator==(const AtlasSettings& other) const = default;
    };
} // namespace nador


#endif //!__NADOR_ATLAS_SETTINGS_H__