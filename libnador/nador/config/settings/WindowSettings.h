#ifndef __NADOR_WINDOW_SETTINGS_H__
#define __NADOR_WINDOW_SETTINGS_H__

#include <string>

#include <glm/glm.hpp>

namespace nador
{
    struct WindowSettings
    {
        std::string appName;
        glm::uvec2  windowDimension = { 0, 0 };
        bool        vSync { true };

        bool showDebugWindow { false };
        bool showDebugInfo { false };

        bool operator==(const WindowSettings& other) const = default;
    };
} // namespace nador


#endif //!__NADOR_WINDOW_SETTINGS_H__