#ifndef __NADOR_I_IMGUI_ADAPTER_H__
#define __NADOR_I_IMGUI_ADAPTER_H__

#include "nador/utils/Types.h"
#include "nador/utils/ImGuiHelper.h"

namespace nador
{
    class IImguiAdapter
    {
    public:
        virtual ~IImguiAdapter() = default;

        virtual bool Imgui_InitImGuiContext(void* nativeWindow, void* context = nullptr) = 0;
	    virtual bool Imgui_NewFrame() = 0;
	    virtual bool Imgui_EndFrame(int32_t width, int32_t height) = 0;
	    virtual bool Imgui_Shutdown() = 0;
        virtual bool Imgui_WantCaptureMouse() const = 0;
        virtual bool Imgui_WantCaptureKeyboard() const = 0;
    };
    CREATE_PTR_TYPES(IImguiAdapter);

    class ImguiAdapter : public IImguiAdapter
    {
    public:
        bool Imgui_InitImGuiContext(void* nativeWindow, void* context = nullptr) override
        {
            return nador::Imgui_InitImGuiContext(nativeWindow, context);
        }

	    bool Imgui_NewFrame() override
        {
            return nador::Imgui_NewFrame();
        }

	    bool Imgui_EndFrame(int32_t width, int32_t height) override
        {
            return nador::Imgui_EndFrame(width, height);
        }

	    bool Imgui_Shutdown() override
        {
            return nador::Imgui_Shutdown();
        }

        bool Imgui_WantCaptureMouse() const override
        {
            return nador::Imgui_WantCaptureMouse();
        }

        bool Imgui_WantCaptureKeyboard() const override
        {
            return nador::Imgui_WantCaptureKeyboard();
        }
    };
} // namespace nador


#endif //!__NADOR_I_IMGUI_ADAPTER_H__