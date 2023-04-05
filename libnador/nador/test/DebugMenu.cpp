#include "imgui.h"

#include "nador/test/DebugMenu.h"
#include "nador/App.h"
#include "nador/test/TestController.h"
#include "DebugMenu.h"

namespace nador
{
    DebugMenu::DebugMenu()
	{
	}

    void DebugMenu::OnDebugRender()
    {
        ITestController* ctrl = IApp::Get()->GetTestController();

        ImGui::SetCursorPos({ 8, 50 });
        for (auto& it : ctrl->GetTests())
        {
            if (ImGui::Button(it.first.c_str()))
            {
                it.second();
            }
        }
	}
}