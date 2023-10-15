#ifndef __NADOR_ATLAS_CONFIG_PARSER_H__
#define __NADOR_ATLAS_CONFIG_PARSER_H__

#include <string>
#include <vector>
#include <optional>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

#include "nador/config/settings/AtlasSettings.h"
#include "nador/system/file/FileData.h"

namespace nador
{
    namespace atlas
    {
        struct AtlasParserException : public std::runtime_error
        {
            AtlasParserException(const std::string& msg)
            : std::runtime_error(msg)
            {
            }
        };

        struct AtlasConfig
        {
            std::string config;
            std::string image;
        };

        void from_json(const nlohmann::json& j, AtlasConfig& c);
        void from_json(const nlohmann::json& j, std::vector<AtlasConfig>& c);

        template <typename ImageIdT>
        struct AtlasImageConfig
        {
            using valueType = ImageIdT;

            ImageIdT    id;
            uint32_t    width;
            uint32_t    height;
            std::string name;
            std::string atlasName;
            glm::mat4x2 uvs;
        };

        template <typename ImageIdT>
        void from_json(const nlohmann::json& j, AtlasImageConfig<ImageIdT>& c)
        {
            c.id        = j["id"].get<ImageIdT>();
            c.width     = j["width"].get<uint32_t>();
            c.height    = j["height"].get<uint32_t>();
            c.name      = j["Name"].get<std::string>();
            c.atlasName = j["atlas_name"].get<std::string>();

            c.uvs[0][0] = j["UV_X_1"].get<float_t>();
            c.uvs[0][1] = j["UV_Y_1"].get<float_t>();

            c.uvs[1][0] = j["UV_X_2"].get<float_t>();
            c.uvs[1][1] = j["UV_Y_2"].get<float_t>();

            c.uvs[2][0] = j["UV_X_3"].get<float_t>();
            c.uvs[2][1] = j["UV_Y_3"].get<float_t>();

            c.uvs[3][0] = j["UV_X_4"].get<float_t>();
            c.uvs[3][1] = j["UV_Y_4"].get<float_t>();
        }

        template <typename T>
        void from_json(const nlohmann::json& j, std::vector<AtlasImageConfig<T>>& c)
        {
            c.clear();
            if (j.contains("images"))
            {
                for (const auto& it : j["images"])
                {
                    c.emplace_back(it.get<atlas::AtlasImageConfig<T>>());
                }
            }
            else
            {
                throw AtlasParserException("std::vector<AtlasConfig> parser error!");
            }
        }

        using AtlasConfigList_t = std::vector<atlas::AtlasConfig>;

        template <typename T>
        using AtlasImageConfigs = std::vector<atlas::AtlasImageConfig<T>>;

        class AtlasConfigParser
        {
        public:
            static AtlasConfigList_t ParseAtlasConfigs(const std::optional<FileData>& atlasConfigData);

            template <typename ImageIdT>
            static AtlasImageConfigs<ImageIdT> ParseAtlasImageConfigs(const std::optional<FileData>& config)
            {
                AtlasImageConfigs<ImageIdT> result;
                if (config && config->IsValid())
                {
                    nlohmann::json json = nlohmann::json::parse(config->Begin(), config->End());
                    json.get_to(result);
                }
                return result;
            }
        };

    } // namespace atlas
} // namespace nador

#endif //!__NADOR_ATLAS_CONFIG_PARSER_H__