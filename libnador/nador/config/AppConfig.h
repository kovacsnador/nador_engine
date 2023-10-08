#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "nador/log/ILog.h"
#include "nador/utils/TinyXmlHelper.h"
#include "nador/config/settings/AtlasSettings.h"
#include "nador/config/settings/WindowSettings.h"
#include "nador/config/settings/VideoSettings.h"

namespace nador
{
    struct AppConfigException : public std::runtime_error
    {
        AppConfigException(const std::string& msg)
        : std::runtime_error(msg)
        {
        }
    };

    struct AppConfig
    {
        WindowSettings windowSettings;
        AtlasSettings atlasSettings;
        VideoSettings videoSettings;

        std::filesystem::path rootFilePath;
        std::filesystem::path uiPath;
        
        bool operator==(const AppConfig& other) const
        {
            return (   windowSettings == other.windowSettings
                    && atlasSettings == other.atlasSettings
                    && videoSettings == other.videoSettings
                    && rootFilePath == other.rootFilePath
                    && uiPath == other.uiPath);
        }
    };

    inline AppConfig Parse(const tinyxml2::XMLDocument& doc)
    {
        AppConfig appConfig;

        const tinyxml2::XMLElement* pRootElement = doc.RootElement();
        if (pRootElement)
        {
            // Get WindowSettings
            const tinyxml2::XMLElement* windowSettings = pRootElement->FirstChildElement("WindowSettings");
            if(windowSettings)
            {
                Parse(windowSettings, appConfig.windowSettings);
            }

            // Get AtlasSettings
            const tinyxml2::XMLElement* atlasSettings = pRootElement->FirstChildElement("AtlasSettings");
            if(atlasSettings)
            {
                Parse(atlasSettings, appConfig.atlasSettings);
            }

            // Get video settings
            const tinyxml2::XMLElement* videoSettings = pRootElement->FirstChildElement("VideoSettings");
            if(videoSettings)
            {
                Parse(videoSettings, appConfig.videoSettings);
            }
            
            appConfig.rootFilePath    = xml::GetByName<std::string>(pRootElement, "RootFilePath");
            appConfig.uiPath          = xml::GetByName<std::string>(pRootElement, "UiPath");
        }

        return appConfig;
    }

    /*!
     * Reads the app config settings from file.
     *
     * \param configPath	The full path of the config file.
     */
    inline AppConfig ReadAppConfigFromFile(std::filesystem::path configPath)
    {
        ENGINE_DEBUG("Read app config file %s.", configPath.string().c_str());

        if(std::filesystem::exists(configPath) == false)
        {
            throw AppConfigException("AppConfig file not exist!");
        }

        tinyxml2::XMLDocument doc;
		doc.LoadFile(configPath.string().c_str());

        return Parse(doc);
    }

    inline AppConfig ReadAppConfig(const std::string& data)
    {
        tinyxml2::XMLDocument doc;
        doc.Parse(data.c_str(), data.size());

        return Parse(doc);
    }

} // namespace nador

#endif // !__APP_CONFIG_H__
