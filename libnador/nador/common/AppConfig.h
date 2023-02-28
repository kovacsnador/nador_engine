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
        std::string texturesPath;
        std::string fontsPath;
        std::string soundsPath;
        std::string uiPath;

        bool showDebugWindow { false };
        bool showDebugInfo { false };
    };

    /*!
     * Reads the app config settings from file.
     *
     * \param configPath	The full path of the config file.
     */
    inline AppConfig ReadAppConfig(const char* configPath)
    {
        ENGINE_DEBUG("Read app config file %s.", configPath);

        AppConfig appConfig;

        tinyxml2::XMLDocument doc;
		doc.LoadFile(configPath);

        tinyxml2::XMLElement* pRootElement = doc.RootElement();
        if (pRootElement)
        {
            appConfig.appName         = xml::GetText(pRootElement, "AppName");
            appConfig.windowDimension = xml::GetUVec2(pRootElement, "WindowDimensions");
            appConfig.vSync           = xml::GetBoolean(pRootElement, "VSync", true);
            appConfig.rootFilePath    = xml::GetText(pRootElement, "RootFilePath");
            appConfig.atlasConfigPath = xml::GetText(pRootElement, "AtlasConfigPath");
            appConfig.atlasImagesPath = xml::GetText(pRootElement, "AtlasImagesPath");
            appConfig.texturesPath    = xml::GetText(pRootElement, "TexturesPath");
            appConfig.fontsPath       = xml::GetText(pRootElement, "FontsPath");
            appConfig.soundsPath      = xml::GetText(pRootElement, "SoundsPath");
            appConfig.uiPath          = xml::GetText(pRootElement, "UiPath");

            appConfig.showDebugWindow = xml::GetBoolean(pRootElement, "ShowDebugWindow", false);
            appConfig.showDebugInfo   = xml::GetBoolean(pRootElement, "ShowDebugInfo", false);
        }

        return appConfig;
    }

} // namespace nador

#endif // !__APP_CONFIG_H__
