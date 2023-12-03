#ifndef __UI_SCENE_H__
#define __UI_SCENE_H__

#include "nador/ui/IUiElement.h"

namespace nador
{
	class UiScene : public IUiElement
	{
	public:
		UiScene();

		UiScene(const glm::ivec2& position, const glm::ivec2& size, UiAlignment aligner, IUiElement* parent = nullptr, bool isShow = true);

		virtual ~UiScene() = default;
	};
}

#endif // !__UI_SCENE_H__
