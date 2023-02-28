#include "nador/ui/UiScene.h"
#include "nador/App.h"

namespace nador
{
	UiScene::UiScene()
	{
		//Default constructor makes a full screen scene
		const IUiApp* app = IApp::Get()->GetUiApp();
		const glm::ivec2& screenSize = app->GetScreenSize();
		SetSize(screenSize);
		SetAlignment({ EHorizontalAlignment::LEFT, EVerticalAlignment::TOP });
		SetPosition({ 0,0 });
		SetParent(nullptr);
	}

	UiScene::UiScene(const glm::ivec2& position, const glm::ivec2& size, UiAlignment aligner, IUiElement* parent, bool isShow)
	: IUiElement(position, size, aligner, parent, isShow)
	{
	}
}