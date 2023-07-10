#ifndef __NADOR_CAMERA_WINDOW_H__
#define __NADOR_CAMERA_WINDOW_H__

#include <string>

#include "imgui.h"

#include "nador/common/Finally.h"
#include "nador/video/renderer/Camera.h"

namespace nador
{
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

            auto position = _camera->GetPosition();
            auto rotation = _camera->GetRotation();

            // Position
            if(ImGui::SliderFloat3("Position", &position.x, -100.0f, 100.0f))
            {
                _camera->SetPosition(position);
            }

            // Rotation
            if(ImGui::SliderFloat3("Rotation", &rotation.x, -180.0f, 180.0f))
            {
                _camera->SetRotation(rotation);
            }
        }

    private:
        Camera*     _camera { nullptr };
        bool        _show { true };
        std::string _windowName;
    };
} // namespace nador

#endif //!__NADOR_CAMERA_WINDOW_H__