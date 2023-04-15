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

        virtual bool InitImGuiContext(void* nativeWindow, void* context = nullptr) = 0;
	    virtual bool NewFrameImGui() = 0;
	    virtual bool EndFrameImGui(int32_t width, int32_t height) = 0;
	    virtual bool ShutdownImGui() = 0;
    };
    CREATE_PTR_TYPES(IImguiAdapter);

    class ImguiAdapter : public IImguiAdapter
    {
    public:
        bool InitImGuiContext(void* nativeWindow, void* context = nullptr) override
        {
            return nador::InitImGuiContext(nativeWindow, context);
        }

	    bool NewFrameImGui() override
        {
            return nador::NewFrameImGui();
        }

	    bool EndFrameImGui(int32_t width, int32_t height) override
        {
            return nador::EndFrameImGui(width, height);
        }

	    bool ShutdownImGui() override
        {
            return nador::ShutdownImGui();
        }
    };
} // namespace nador


#endif //!__NADOR_I_IMGUI_ADAPTER_H__