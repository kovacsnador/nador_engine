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
            const auto& mtx = _camera->GetCameraMtx();

            // Position
            if(ImGui::SliderFloat3("Position", &position.x, -100.0f, 100.0f))
            {
                _camera->SetPosition(position);
            }

            // Rotation
            if(ImGui::SliderFloat3("Rotation", &rotation.x, -90.0f, 90.0f))
            {
                _camera->SetRotation(rotation);
            }

            const auto& l0 = mtx[0];
            const auto& l1 = mtx[1];
            const auto& l2 = mtx[2];
            const auto& l3 = mtx[3];

            ImGui::LabelText("Camera Matrix", "%.4f, %.4f, %.4f, %.4f\n %.4f, %.4f, %.4f, %.4f\n%.4f, %.4f, %.4f, %.4f\n%.4f, %.4f, %.4f, %.4f",
                    l0.x, l0.y, l0.z, l0.w,
                    l1.x, l1.y, l1.z, l1.w,
                    l2.x, l2.y, l2.z, l2.w,
                    l3.x, l3.y, l3.z, l3.w);
        }

    private:
        Camera*     _camera { nullptr };
        bool        _show { true };
        std::string _windowName;
    };
} // namespace nador

#endif //!__NADOR_CAMERA_WINDOW_H__