#ifndef __NADOR_CAMERA_WINDOW_H__
#define __NADOR_CAMERA_WINDOW_H__

#include <string>

#include "imgui.h"

#include "nador/common/Finally.h"

namespace nador
{
    class Camera;

    class CameraWindow
    {
    public:
        CameraWindow(std::string windowName, Camera* camera)
        : _camera(camera)
        , _windowName(std::move(windowName))
        {
        }

        void SetCamera(Camera* camera) { _camera = camera; }

        void Toggle() { _show = !_show; }

        void OnDebugRender() const
        {
            if (_show == false || _camera == nullptr)
            {
                return;
            }

            // setup rendering context
            ImGui::Begin("Camera Settings");
            auto imguiRenderEnd = Finally([] { ImGui::End(); });

            // render the window
            ImGui::Text("This is the Camera setting window");
        }

    private:
        Camera*     _camera { nullptr };
        bool        _show { true };
        std::string _windowName;
    };
} // namespace nador

#endif //!__NADOR_CAMERA_WINDOW_H__