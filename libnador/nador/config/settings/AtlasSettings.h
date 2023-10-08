#ifndef __NADOR_ATLAS_SETTINGS_H__
#define __NADOR_ATLAS_SETTINGS_H__

#include <filesystem>

#include "nador/utils/TinyXmlHelper.h"

namespace nador
{
    struct AtlasSettings
    {
        std::filesystem::path atlasConfigPath;
        std::filesystem::path atlasImagesPath;
        size_t                atlasCacheSize;

        bool operator==(const AtlasSettings& other) const = default;
    };

    inline void Parse(const tinyxml2::XMLElement* elem, AtlasSettings& settings)
    {
        settings.atlasConfigPath = xml::GetByName<std::string>(elem, "AtlasConfigPath");
        settings.atlasImagesPath = xml::GetByName<std::string>(elem, "AtlasImagesPath");
        settings.atlasCacheSize  = xml::GetByName<size_t>(elem, "AtlasCacheSize");
    }

} // namespace nador

#endif //!__NADOR_ATLAS_SETTINGS_H__