#include "imgui.h"

#include "nador/test/test_views/UiSquareTest.h"
#include "nador/test/TestCommon.h"
#include "nador/video/IVideo.h"
#include "nador/system/IFileController.h"

namespace nador
{
	TestImage::TestImage()
	{
		material.uColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		renderData.indices = {
			0, 1, 2,
			2, 3, 0,
		};

		renderData.texCoords = {
			{0.0f, 0.0f},
			{1.0f, 0.0f },
			{1.0f, 1.0f},
			{0.0f, 1.0f },
		};
	}

	glm::mat4 TestImage::Update(UiAlignment aligner, const glm::mat4& parent)
	{
		glm::mat4 vertices = aligner.GenerateVertices(pos, size, parent);
		renderData.SetVertices(vertices);
		return vertices;
	}

	UiSquareTest::UiSquareTest(const IVideo* video, const IFileController* fileCtrl, const IUiApp* uiApp)
	: _video(video)
	, _fileCtrl(fileCtrl)
	, _uiApp(uiApp)
	{
		_parent.material.texture.reset(new Texture(_video, _fileCtrl->Read("res/textures/playerBtn.png")));
		_child.material.texture.reset(new Texture(_video, _fileCtrl->Read("res/textures/test.png")));

		_parent.pos = { 100, 100 };
		_parent.size = { 500, 500 };

		_child.pos = { 50, 50 };
		_child.size = { 200, 200 };

		_Update();
	}

	void UiSquareTest::OnRender(IRenderer* renderer)
	{
		glm::mat4 modelMtx(1.0f);

		renderer->Draw(&_parent.material, _parent.renderData, &modelMtx);
		renderer->Draw(&_child.material, _child.renderData, &modelMtx);
	}

	void UiSquareTest::OnDebugRender(IRenderer* /*renderer*/)
	{
		static const char* horizontal[]{ "LEFT", "RIGHT", "CENTER", "STRETCH"};
		static const char* vertical[]{ "TOP", "BOTTOM", "CENTER", "STRETCH" };

		bool needUpdate = false;

		needUpdate |= ImGui::SliderInt2("Parent pos X Y", &_parent.pos.x, 0, 500);
		needUpdate |= ImGui::SliderInt2("Parent size X Y", &_parent.size.x, 0, 500);
		needUpdate |= ImGui::Combo("Parent horizontal", &_parentSelectedHorizontal, horizontal, IM_ARRAYSIZE(horizontal));
		needUpdate |= ImGui::Combo("Parent vertical", &_parentSelectedVertical, vertical, IM_ARRAYSIZE(vertical));

		ImGui::NewLine();
		needUpdate |= ImGui::SliderInt2("Child pos X Y", &_child.pos.x, 0, 500);
		needUpdate |= ImGui::SliderInt2("Child size X Y", &_child.size.x, 0, 500);
		needUpdate |= ImGui::Combo("Child horizontal", &_childSelectedHorizontal, horizontal, IM_ARRAYSIZE(horizontal));
		needUpdate |= ImGui::Combo("Child vertical", &_childSelectedVertical, vertical, IM_ARRAYSIZE(vertical));


		if (needUpdate)
		{
			_Update();
		}
	}

	void UiSquareTest::_Update()
	{
		auto screenVertices = _uiApp->GetScreenVertices();

		auto parentAligner = ToAligner(_parentSelectedHorizontal, _parentSelectedVertical);
		auto parentVerices = _parent.Update(parentAligner, screenVertices);

		auto childAligner = ToAligner(_childSelectedHorizontal, _childSelectedVertical);
		_child.Update(childAligner, parentVerices);
	}
}