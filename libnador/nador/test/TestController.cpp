#include "imgui.h"

#include "nador/App.h"
#include "nador/test/TestController.h"
#include "nador/test/DebugMenu.h"
#include "nador/test/Tests.h"

namespace nador
{
	TestController::TestController()
	: _onTickListener(&g_onTickEvent, std::bind(&TestController::OnTick, this, arg::_1))
	, _onRenderListener(&g_onRenderEvent, std::bind(&TestController::OnRender, this))
	, _onDebugRenderListener(&g_onDebugRenderEvent, std::bind(&TestController::OnDebugRender, this))
	{
		_currentTest.reset(new DebugMenu);

		AddTest<ClearColorTest>("Clear Color");
		AddTest<BaseMaterialTest>("Base Material Test");
		AddTest<RoundEdgeMaterialTest>("Round Edge Material Test");
		AddTest<SimpleLineTest>("Simple Line Test");
		AddTest<TextureTest>("Texture Test");
		AddTest<AtlasTest>("Atlas Test");
		AddTest<FontTest>("Font Test");
		AddTest<InputTest>("Input Test");
		AddTest<SoundTest>("Sound Test");
		AddTest<UiSquareTest>("UiSquare Test");
		AddTest<UiElementsTest>("UiElements Test");
	}

	void TestController::OnTick(float_t deltaTime)
	{
		_currentTest->OnTick(deltaTime);
	}

	void TestController::OnRender()
	{
		IRenderer* renderer = IApp::Get()->GetRenderer();
		_currentTest->OnRender(renderer);
	}

	void TestController::OnDebugRender()
	{
		if(dynamic_cast<DebugMenu*>(_currentTest.get()) == nullptr)
		{
			if(ImGui::Button("<-"))
			{
				_currentTest.reset(new DebugMenu);
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
}