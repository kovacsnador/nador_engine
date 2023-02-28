#ifndef __IMGUI_HELPER_H__
#define __IMGUI_HELPER_H__

#include "nador/utils/Types.h"


namespace nador
{
	bool ImGuiStrContainerIter(void* vec, int idx, const char** out_text);

    bool InitImGuiContext(void* nativeWindow, void* context = nullptr);
	bool NewFrameImGui();
	bool EndFrameImGui(int32_t width, int32_t height);
	bool ShutdownImGui();
}


#endif // !__IMGUI_HELPER_H__
