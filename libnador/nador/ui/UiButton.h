#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "nador/ui/UiImage.h"
#include "nador/ui/UiTextLabel.h"

namespace nador
{
	class UiButton : public IUiElement
	{
	public:
		static constexpr glm::vec3 DEFAULT_SCALE_ON_DOWN = { 0.95f, 0.95f, 1 };

		enum class EState
		{
			IDLE,
			DOWN,
			OVER,
		};

		struct UiButtonImages
		{
			video::EImageName idle;
			video::EImageName down;
			video::EImageName over;
		};

		UiButton(const glm::ivec2& position, const glm::ivec2& size, UiButtonImages images, UiAlignment aligner = {}, IUiElement* parent = nullptr, bool isShow = true);

		void SetUiTextLabel(const FontPtr& font, std::string_view text);
		UiTextLabelPtr GetUiTextLabel() { return _uiTextLabelPtr; }

		void SetScaleOnButtonDown(const glm::vec3& onDownScale);

	protected:
		void OnMousePressed(EMouseButton mouseButton, const glm::vec2& position) override;
		void OnMouseReleased(EMouseButton mouseButton, const glm::vec2& position) override;

		void OnTick(IUiLogicState* uiLogicState) override;

	private:
		EState			_state{ EState::IDLE };
		UiImage			_uiImageBackground;
		UiButtonImages	_images;

		glm::vec3 _scaleOnButtonDown { DEFAULT_SCALE_ON_DOWN };

		UiTextLabelPtr	_uiTextLabelPtr{ nullptr };
	};
	CREATE_PTR_TYPES(UiButton);
}

#endif // !__UI_BUTTON_H__
