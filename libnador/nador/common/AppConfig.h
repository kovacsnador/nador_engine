#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include <string>
#include <filesystem>

#include <glm/glm.hpp>

#include "nador/log/ILog.h"
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
        std::filesystem::path atlasConfigPath;
        std::filesystem::path atlasImagesPath;

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
                appConfig.windowSettings.appName         = xml::GetByName<std::string>(windowSettings, "AppName");
                appConfig.windowSettings.windowDimension = xml::GetByName<glm::uvec2>(windowSettings, "WindowDimensions");
                appConfig.windowSettings.vSync           = xml::GetByName<bool>(windowSettings, "VSync", true);
                appConfig.windowSettings.showDebugWindow = xml::GetByName<bool>(windowSettings, "ShowDebugWindow", false);
                appConfig.windowSettings.showDebugInfo   = xml::GetByName<bool>(windowSettings, "ShowDebugInfo", false);
            }

            // Get AtlasSettings
            const tinyxml2::XMLElement* atlasSettings = pRootElement->FirstChildElement("AtlasSettings");
            if(atlasSettings)
            {
                appConfig.atlasSettings.atlasConfigPath = xml::GetByName<std::string>(atlasSettings, "AtlasConfigPath");
                appConfig.atlasSettings.atlasImagesPath = xml::GetByName<std::string>(atlasSettings, "AtlasImagesPath");
            }

            // Get video settings
            const tinyxml2::XMLElement* videoSettings = pRootElement->FirstChildElement("VideoSettings");
            if(videoSettings)
            {
                appConfig.videoSettings.maxVertexCount = xml::GetByName<uint32_t>(videoSettings, "MaxVertexCount");
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
            throw std::runtime_error("AppConfig file not exist!");
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
