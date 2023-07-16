#ifndef __IMGUI_HELPER_H__
#define __IMGUI_HELPER_H__

#include <tuple>

#include "nador/utils/Types.h"

namespace nador
{
	bool Imgui_StrContainerIter(void* vec, int idx, const char** out_text);

    bool Imgui_InitImGuiContext(void* nativeWindow, void* context = nullptr);
	bool Imgui_NewFrame();
	bool Imgui_EndFrame(int32_t width, int32_t height);
	bool Imgui_Shutdown();
    bool Imgui_WantCaptureMouse();
    bool Imgui_WantCaptureKeyboard();

	template<typename ContainerTy, typename PredTy>
	bool ImGuiTupleContainerIter(void* pairIn, int idx, const char** out_text)
    {
		auto tuple = reinterpret_cast<std::tuple<ContainerTy, PredTy>*>(pairIn);
		const auto& container = std::get<ContainerTy>(*tuple);
		const auto& pred = std::get<PredTy>(*tuple);

        if (idx < 0 || (size_t)idx >= container.size())
        {
            return false;
        }
        *out_text = pred(container, idx);
        return true;
    }
}


#endif // !__IMGUI_HELPER_H__
