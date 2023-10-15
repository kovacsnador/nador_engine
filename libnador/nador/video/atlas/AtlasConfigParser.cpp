#include "nador/video/atlas/AtlasConfigParser.h"

namespace nador
{
    void atlas::from_json(const nlohmann::json& j, AtlasConfig& c)
    {
        if (j.contains("c_name") && j.contains("i_name"))
        {
            j.at("c_name").get_to(c.config);
            j.at("i_name").get_to(c.image);
        }
        else
        {
            throw atlas::AtlasParserException("AtlasConfig parser error!");
        }
    }

    void atlas::from_json(const nlohmann::json& j, std::vector<AtlasConfig>& c)
    {
        c.clear();
        if (j.contains("atlas_names"))
        {
            for (const auto& it : j["atlas_names"])
            {
                c.emplace_back(it.template get<atlas::AtlasConfig>());
            }
        }
        else
        {
            throw atlas::AtlasParserException("std::vector<AtlasConfig> parser error!");
        }
    }

    atlas::AtlasConfigList_t atlas::AtlasConfigParser::ParseAtlasConfigs(const std::optional<FileData>& atlasConfigData)
    {
        atlas::AtlasConfigList_t result;
        if (atlasConfigData && atlasConfigData->IsValid())
        {
            nlohmann::json j = nlohmann::json::parse(atlasConfigData->Begin(), atlasConfigData->End());
            j.get_to(result);
        }
        return result;
    }
} // namespace nador
