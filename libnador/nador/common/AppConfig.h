#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include <string>

#include <glm/glm.hpp>

#include "nador/log/Log.h"
#include "nador/utils/TinyXmlHelper.h"

namespace nador
{
    struct WindowSettings
    {
        std::string appName;
        glm::uvec2  windowDimension = { 0, 0 };
        bool        vSync { true };

        bool showDebugWindow { false };
        bool showDebugInfo { false };

        bool operator==(const WindowSettings& other) const
        {
            return (appName == other.appName 
                    && windowDimension == other.windowDimension
                    && vSync == other.vSync
                    && showDebugInfo == other.showDebugInfo
                    && showDebugWindow == other.showDebugWindow);
        }
    };

    struct AtlasSettings
    {
        std::string atlasConfigPath;
        std::string atlasImagesPath;

        bool operator==(const AtlasSettings& other) const
        {
            return (   atlasConfigPath == other.atlasConfigPath
                    && atlasImagesPath == other.atlasImagesPath);
        }
    };

    struct VideoSettings
    {
        size_t maxVertexCount;

        bool operator==(const VideoSettings& other) const
        {
            return (maxVertexCount == other.maxVertexCount);
        }
    };

    struct AppConfig
    {
        WindowSettings windowSettings;
        AtlasSettings atlasSettings;
        VideoSettings videoSettings;

        std::string rootFilePath;
        std::string uiPath;

        
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
                appConfig.windowSettings.appName         = xml::GetText(windowSettings, "AppName");
                appConfig.windowSettings.windowDimension = xml::GetUVec2(windowSettings, "WindowDimensions");
                appConfig.windowSettings.vSync           = xml::GetBoolean(windowSettings, "VSync", true);
                appConfig.windowSettings.showDebugWindow = xml::GetBoolean(windowSettings, "ShowDebugWindow", false);
                appConfig.windowSettings.showDebugInfo   = xml::GetBoolean(windowSettings, "ShowDebugInfo", false);
            }

            // Get AtlasSettings
            const tinyxml2::XMLElement* atlasSettings = pRootElement->FirstChildElement("AtlasSettings");
            if(atlasSettings)
            {
                appConfig.atlasSettings.atlasConfigPath = xml::GetText(atlasSettings, "AtlasConfigPath");
                appConfig.atlasSettings.atlasImagesPath = xml::GetText(atlasSettings, "AtlasImagesPath");
            }

            // Get video settings
            const tinyxml2::XMLElement* videoSettings = pRootElement->FirstChildElement("VideoSettings");
            if(videoSettings)
            {
                appConfig.videoSettings.maxVertexCount = xml::GetUint32T(videoSettings, "MaxVertexCount");
            }
            
            appConfig.rootFilePath    = xml::GetText(pRootElement, "RootFilePath");
            appConfig.uiPath          = xml::GetText(pRootElement, "UiPath");
        }

        return appConfig;
    }

    /*!
     * Reads the app config settings from file.
     *
     * \param configPath	The full path of the config file.
     */
    inline AppConfig ReadAppConfigFromFile(const char* configPath)
    {
        ENGINE_DEBUG("Read app config file %s.", configPath);

        tinyxml2::XMLDocument doc;
		doc.LoadFile(configPath);

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
