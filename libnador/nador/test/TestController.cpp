#include "imgui.h"

#include "nador/test/TestController.h"
#include "nador/test/Tests.h"

namespace nador
{
    TestController::TestController()
    : _onTickListener(&g_onTickEvent, std::bind(&TestController::OnTick, this, arg::_1))
    , _onRenderListener(&g_onRenderEvent, std::bind(&TestController::OnRender, this, arg::_1))
    , _onDebugRenderListener(&g_onDebugRenderEvent, std::bind(&TestController::OnDebugRender, this, arg::_1))
    {
    }

    void TestController::OnTick(float_t deltaTime)
    {
        if (_currentTest)
        {
            _currentTest->OnTick(deltaTime);
        }
    }

    void TestController::OnRender(IRenderer* renderer)
    {
        if (_currentTest)
        {
            _currentTest->OnRender(renderer);
        }
    }

    void TestController::OnDebugRender(IRenderer* renderer)
    { 
        if (_currentTest)
        {
            if (ImGui::Button("<-"))
            {
                _currentTest.reset();
                _DebugRenderStartMenu();
            }
            else
            {
                _currentTest->OnDebugRender();
            }
        }
        else
        {
            _DebugRenderStartMenu();
        }

        ImGui::SetCursorPos({ 150, 20 });
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestController::_DebugRenderStartMenu()
    {
        if (ImGui::Button("Toggle Debug Info"))
        {
            if (_toggleDebugTxtCb)
            {
                _toggleDebugTxtCb();
            }
        }

        ImGui::SetCursorPos({ 8, 50 });
        for (auto& it : _tests)
        {
            if (ImGui::Button(it.first.c_str()))
            {
                it.second();
            }
        }
    }

    void TestController::Suspend(bool suspend)
    {
        _onRenderListener.Suspend(suspend);
        _onDebugRenderListener.Suspend(suspend);
        _onTickListener.Suspend(suspend);
    }

    void TestController::SetToggleDebugTextCallback(ToggleDebugTextCb_t cb) noexcept
    {
        _toggleDebugTxtCb = cb;
    }

} // namespace nador