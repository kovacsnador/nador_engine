#ifndef __NADOR_WINDOW_SETTINGS_H__
#define __NADOR_WINDOW_SETTINGS_H__

#include <string>

#include <glm/glm.hpp>

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
            return appName == other.appName &&
                    windowDimension == other.windowDimension &&
                    vSync == other.vSync &&
                    showDebugWindow == other.showDebugWindow &&
                    showDebugInfo == other.showDebugInfo;
        }
    };

    inline void Parse(const tinyxml2::XMLElement* elem, WindowSettings& settings)
    {
        settings.appName         = xml::GetByName<std::string>(elem, "AppName");
        settings.windowDimension = xml::GetByName<glm::uvec2>(elem, "WindowDimensions");
        settings.vSync           = xml::GetByName<bool>(elem, "VSync", true);
        settings.showDebugWindow = xml::GetByName<bool>(elem, "ShowDebugWindow", false);
        settings.showDebugInfo   = xml::GetByName<bool>(elem, "ShowDebugInfo", false);
    }

} // namespace nador

#endif //!__NADOR_WINDOW_SETTINGS_H__