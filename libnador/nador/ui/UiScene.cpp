#include "nador/ui/UiScene.h"

namespace nador
{
	UiScene::UiScene(const glm::ivec2& position, const glm::ivec2& size, UiAlignment aligner, IUiElement* parent, bool isShow)
	: IUiElement(position, size, aligner, parent, isShow)
	{
	}
}