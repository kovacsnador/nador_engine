#include "imgui.h"

#include "nador/utils/ImGuiHelper.h"
#include "nador/test/test_views/AtlasTest.h"
#include "EImageName.h"
#include "nador/video/atlas/IAtlasController.h"
#include "nador/video/renderer/IRenderer.h"

namespace nador
{
	AtlasTest::AtlasTest(const IAtlasController* atlasCtrl)
	: _atlasCtrl(atlasCtrl)
	{
		ImageData imageData = _atlasCtrl->GetImage(video::EImageName::TEST145);

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
		renderer->Draw(&_material, _renderData, modelMatrix);
	}

	void AtlasTest::OnDebugRender(IRenderer* /*renderer*/)
	{
		using LisboxPred_t = std::function<const char*(const strings_t&, size_t)>;
		auto ListStrCallback = [](const strings_t& c, size_t i) { return c.at(i).c_str(); };

		static int32_t selectedItemAtlas = 0;
		static int32_t selectedItemImage = 0;

		strings_t atlasNames = _atlasCtrl->GetAtlasNames();
		IAtlasController::atlases_t atlases = _atlasCtrl->GetAtlases();
		
		ImGui::ListBox("Atlases", &selectedItemAtlas,
					   Imgui_StrContainerIter,
					   reinterpret_cast<void*>(&atlasNames), atlasNames.size(), 5);

		if((size_t)selectedItemAtlas < atlases.size())
		{
			AtlasPtr selectedAtlas = atlases.at(selectedItemAtlas);
			
			strings_t imageNames = selectedAtlas->GetImageNames();
			auto imagesTuple = std::tuple<strings_t, LisboxPred_t>(imageNames, ListStrCallback);

			ImGui::ListBox("Images", &selectedItemImage,
						   &ImGuiTupleContainerIter<strings_t, LisboxPred_t>,
						   reinterpret_cast<void*>(&imagesTuple), imageNames.size(), 10);


			const auto& ids = selectedAtlas->GetImageIds();
			if((size_t)selectedItemImage >= ids.size())
			{
				selectedItemImage = 0;
			}
			if(ids.size())
			{
				video::EImageName id = ids.at(selectedItemImage);
				ImageData imageData = _atlasCtrl->GetImage(id);

				_material.texture = imageData.texture;
				_renderData.SetTextureCoords(imageData.image->uvs);
			}
		}
	}
}