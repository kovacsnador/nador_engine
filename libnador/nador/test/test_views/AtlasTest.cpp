#include "imgui.h"

#include "nador/utils/ImGuiHelper.h"
#include "nador/test/test_views/AtlasTest.h"
#include "nador/App.h"
#include "EImageName.h"

namespace nador
{
	AtlasTest::AtlasTest()
	{
		const IAtlasController* atlasCtrl = IApp::Get()->GetAtlasController();

		ImageData imageData = atlasCtrl->GetImage(video::EImageName::TEST145);

		_material.uColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		_material.texture = imageData.texture;

		_renderData.vertices = {
			{0.0f,   0.0f,   1.0f, 1.0f},
			{300.0f, 0.0f,   1.0f, 1.0f },
			{300.0f, 300.0f, 1.0f, 1.0f},
			{0.0f,   300.0f, 1.0f, 1.0f },
		};

		_renderData.indices = {
			0, 1, 2,
			2, 3, 0,
		};

		_renderData.SetTextureCoords(imageData.image->uvs);
	}

	void AtlasTest::OnRender(IRenderer* renderer)
	{
		glm::mat4 modelMatrix(1.0f);
		renderer->Draw(&_material, _renderData, &modelMatrix);
	}

	void AtlasTest::OnDebugRender()
	{
		static int32_t selectedItemAtlas = 0;
		static int32_t selectedItemImage = 0;

		const IAtlasController* atlasCtrl = IApp::Get()->GetAtlasController();

		strings_t atlasNames = atlasCtrl->GetAtlasNames();
		IAtlasController::atlases_t atlases = atlasCtrl->GetAtlases();
		
		ImGui::ListBox("Atlases", &selectedItemAtlas,
					   ImGuiStrContainerIter,
					   reinterpret_cast<void*>(&atlasNames), atlasNames.size(), 5);

		if((size_t)selectedItemAtlas < atlases.size())
		{
			AtlasPtr selectedAtlas = atlases.at(selectedItemAtlas);

			strings_t imageNames = selectedAtlas->GetImageNames();

			ImGui::ListBox("Images", &selectedItemImage,
						   ImGuiStrContainerIter,
						   reinterpret_cast<void*>(&imageNames), imageNames.size(), 10);

			const auto& ids = selectedAtlas->GetImageIds();
			if((size_t)selectedItemImage >= ids.size())
			{
				selectedItemImage = 0;
			}
			if(ids.size())
			{
				video::EImageName id = ids.at(selectedItemImage);
				ImageData imageData = selectedAtlas->GetImageData(id);

				_material.texture = imageData.texture;
				_renderData.SetTextureCoords(imageData.image->uvs);
			}
		}
	}
}