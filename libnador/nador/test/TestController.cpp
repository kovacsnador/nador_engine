#include "imgui.h"

#include "nador/App.h"
#include "nador/test/TestController.h"
#include "nador/test/DebugMenu.h"
#include "nador/test/Tests.h"

namespace nador
{
    TestController::TestController(const IVideo*           video,
                                   const IFileController*  fileCtrl,
                                   const IAtlasController* atlasCtrl,
                                   const IFontController*  fontCtrl,
                                   IUiApp*                 uiApp,
                                   const IInputController* inputCtrl)
    : _onTickListener(&g_onTickEvent, std::bind(&TestController::OnTick, this, arg::_1))
    , _onRenderListener(&g_onRenderEvent, std::bind(&TestController::OnRender, this, arg::_1))
    , _onDebugRenderListener(&g_onDebugRenderEvent, std::bind(&TestController::OnDebugRender, this, arg::_1))
    , _video(video)
    , _fileCtrl(fileCtrl)
    {
        _currentTest.reset(new DebugMenu());

        /*AddTest<ClearColorTest>("Clear Color", _video);
        AddTest<BaseMaterialTest>("Base Material Test");
        AddTest<RoundEdgeMaterialTest>("Round Edge Material Test");
        AddTest<SimpleLineTest>("Simple Line Test");
        AddTest<TextureTest>("Texture Test", _video, _fileCtrl);
        AddTest<AtlasTest>("Atlas Test", atlasCtrl);
        AddTest<FontTest>("Font Test", fontCtrl);
        AddTest<InputTest>("Input Test", inputCtrl);
        AddTest<SoundTest>("Sound Test");
        AddTest<UiSquareTest>("UiSquare Test", _video, _fileCtrl);
        AddTest<UiElementsTest>("UiElements Test", uiApp, fontCtrl, inputCtrl);*/
    }

    void TestController::OnTick(float_t deltaTime)
    {
        _currentTest->OnTick(deltaTime);
    }

    void TestController::OnRender(IRenderer* renderer)
    {
        _currentTest->OnRender(renderer);
    }

    void TestController::OnDebugRender(IRenderer* renderer)
    {
        if (dynamic_cast<DebugMenu*>(_currentTest.get()) == nullptr)
        {
            if (ImGui::Button("<-"))
            {
                _currentTest.reset(new DebugMenu());
            }
        }
        else
        {
            if (ImGui::Button("Toggle Debug Info"))
            {
                IApp* app = IApp::Get();
                app->ShowDebugInfo(!app->IsShowDebugInfo());
            }
        }

        _currentTest->OnDebugRender();

        ImGui::SetCursorPos({ 150, 20 });
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestController::Suspend(bool suspend)
    {
        _onRenderListener.Suspend(suspend);
        _onDebugRenderListener.Suspend(suspend);
        _onTickListener.Suspend(suspend);
    }
} // namespace nador