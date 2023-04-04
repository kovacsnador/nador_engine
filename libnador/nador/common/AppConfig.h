#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include <string>

#include <glm/glm.hpp>

#include "nador/log/Log.h"
#include "nador/utils/TinyXmlHelper.h"

namespace nador
{
    struct AppConfig
    {
        std::string appName;
        glm::uvec2  windowDimension = { 0, 0 };
        bool        vSync { true };
        std::string rootFilePath;
        std::string atlasConfigPath;
        std::string atlasImagesPath;
        std::string uiPath;

        bool showDebugWindow { false };
        bool showDebugInfo { false };

        bool operator==(const AppConfig& other) const
        {
            return (appName == other.appName 
                    && windowDimension == other.windowDimension
                    && vSync == other.vSync
                    && rootFilePath == other.rootFilePath
                    && atlasConfigPath == other.atlasConfigPath
                    && atlasImagesPath == other.atlasImagesPath
                    && uiPath == other.uiPath
                    && showDebugInfo == other.showDebugInfo
                    && showDebugWindow == other.showDebugWindow);
        }
    };

    inline AppConfig Parse(const tinyxml2::XMLDocument& doc)
    {
        AppConfig appConfig;

        const tinyxml2::XMLElement* pRootElement = doc.RootElement();
        if (pRootElement)
        {
            appConfig.appName         = xml::GetText(pRootElement, "AppName");
            appConfig.windowDimension = xml::GetUVec2(pRootElement, "WindowDimensions");
            appConfig.vSync           = xml::GetBoolean(pRootElement, "VSync", true);
            appConfig.rootFilePath    = xml::GetText(pRootElement, "RootFilePath");
            appConfig.atlasConfigPath = xml::GetText(pRootElement, "AtlasConfigPath");
            appConfig.atlasImagesPath = xml::GetText(pRootElement, "AtlasImagesPath");
            appConfig.uiPath          = xml::GetText(pRootElement, "UiPath");
            appConfig.showDebugWindow = xml::GetBoolean(pRootElement, "ShowDebugWindow", false);
            appConfig.showDebugInfo   = xml::GetBoolean(pRootElement, "ShowDebugInfo", false);
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
