#include "imgui.h"

#include "nador/test/test_views/ClearColorTest.h"
#include "nador/App.h"

float_t nador::ClearColorTest::_clearColor[4];

namespace nador
{
	ClearColorTest::ClearColorTest()
	{
	}

	void ClearColorTest::OnRender(IRenderer*)
	{
		const IVideo* video = IApp::Get()->GetVideo();

		video->ClearColorRGBA(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
	}

	void ClearColorTest::OnDebugRender()
	{
		ImGui::ColorEdit4("Clear Color", _clearColor);
	}
}