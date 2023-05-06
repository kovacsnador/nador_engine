#ifndef __IMGUI_HELPER_H__
#define __IMGUI_HELPER_H__

#include <tuple>

#include "nador/utils/Types.h"

namespace nador
{
	bool ImGuiStrContainerIter(void* vec, int idx, const char** out_text);

    bool InitImGuiContext(void* nativeWindow, void* context = nullptr);
	bool NewFrameImGui();
	bool EndFrameImGui(int32_t width, int32_t height);
	bool ShutdownImGui();
    bool WantCaptureMouse();

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
