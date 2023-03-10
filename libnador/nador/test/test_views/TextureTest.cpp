#include "imgui.h"

#include "nador/test/test_views/TextureTest.h"
#include "nador/App.h"

namespace nador
{
	TextureTest::TextureTest()
	{
		_material.uColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		_material.texture.reset(new Texture("res/textures/test.png"));

		_renderData.vertices = {
			{0.0f,   0.0f,   0.0f, 1.0f},
			{100.0f, 0.0f,   0.0f, 1.0f},
			{100.0f, 100.0f, 0.0f, 1.0f},
			{0.0f,   100.0f, 0.0f, 1.0f},
		};

		_renderData.indices = {
			0, 1, 2,
			2, 3, 0,
		};

		_renderData.texCoords = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f},
		};

		_renderData2 = _renderData;

		_renderData2.vertices = {
			{110.0f, 0.0f,   0.0f, 1.0f},
			{210.0f, 0.0f,   0.0f, 1.0f},
			{210.0f, 100.0f, 0.0f, 1.0f},
			{110.0f, 100.0f, 0.0f, 1.0f},
		};

		_renderData3 = _renderData;

		_renderData3.vertices = {
			{210.0f, 0.0f,   0.0f, 1.0f},
			{310.0f, 0.0f,   0.0f, 1.0f},
			{310.0f, 100.0f, 0.0f, 1.0f},
			{210.0f, 100.0f, 0.0f, 1.0f},
		};
	}

	void TextureTest::OnRender(IRenderer* renderer)
	{
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), _translation);

		renderer->Draw(&_material, _renderData, &modelMatrix);
		renderer->Draw(&_material, _renderData2, &modelMatrix);
		renderer->Draw(&_material, _renderData3, &modelMatrix);
	}

	void TextureTest::OnDebugRender()
	{
		const IRenderer* renderer = IApp::Get()->GetRenderer();

		const glm::ivec2& screenSize = renderer->GetScreenSize();

		ImGui::SliderFloat("Translation X", &_translation.x, 0.0f, (float)screenSize.x);
		ImGui::SliderFloat("Translation Y", &_translation.y, 0.0f, (float)screenSize.y);
		ImGui::SliderFloat("Translation Z", &_translation.z, 0.0f, 10.0f);
	}
}