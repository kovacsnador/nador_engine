#ifndef __NADOR_UI_EDIT_TEXT_H__
#define __NADOR_UI_EDIT_TEXT_H__

#include "nador/ui/IUiElement.h"
#include "nador/ui/UiImage.h"
#include "nador/ui/UiTextLabel.h"
#include "nador/ui/UiOverlay.h"
#include "nador/utils/Time.h"

namespace nador
{
	class IRenderer;

	class UiEditText : public IUiElement
	{
		enum class EState
		{
			IDLE,
			EDIT,
			OVER,
		};

	public:
		struct UiEditTextImages
		{
			video::EImageName idle;
			video::EImageName edit;
			video::EImageName over;
		};

		UiEditText(const glm::ivec2& position, const glm::ivec2& size, const FontPtr& font, UiAlignment alignment = {}, IUiElement* parent = nullptr);
		UiEditText(const glm::ivec2& position, const glm::ivec2& size, UiEditTextImages images, const FontPtr& font, UiAlignment alignment = {}, IUiElement* parent = nullptr);

		UiImage& GetBackgroundImage();
		UiTextLabel& GetTextLabel();
		UiOverlay& GetBottomLine();
		UiOverlay& GetCursor();

		void SetCursorBlinkTime(float_t time);
		void SetDefaultLineWidth(int32_t lineWidth);

	protected:
		void OnRender(IRenderer* renderer, const quadVertices_t& vertices) override;
		void OnRenderEnd(IRenderer* renderer) override;

		void OnMouseReleased(EMouseButton mouseButton, const glm::vec2& position) override;
		bool OnCharPressed(const std::string& unicode) override;

		bool OnKeyPressed(EKeyCode keyCode) override;
		bool OnKeyHolded(EKeyCode keyCode) override;

		void OnTick(IUiLogicState* uiLogicState, bool mouseOver) override;

	private:
		static constexpr int32_t DEFAULT_LINE_WIDTH = 2;

		void _UpdateBackground(EState state);
		void _UpdateCursor(EState state, float_t deltaTime);
		void _UpdateTextSliding();

		void _UpdateLineWidth(int32_t lineWidth);
		int32_t _CalculateTextLabelOffset();

		EState			 _state{ EState::IDLE };
		UiEditTextImages _images{ video::EImageName::IMAGE_NAME_COUNT, video::EImageName::IMAGE_NAME_COUNT , video::EImageName::IMAGE_NAME_COUNT };
		UiImage			 _uiImageBackground;

		UiOverlay		 _bottomLine;
		UiOverlay		 _cursor;
		
		UiTextLabel		_uiTextLabel;

		float_t	_lastCursorBlinking{ 0.0f };
		float_t _cursorBlinkTime{ 0.4f };

		int32_t _lineWidth{ DEFAULT_LINE_WIDTH };
	};
	CREATE_PTR_TYPES(UiEditText);
}

#endif // !__NADOR_UI_EDIT_TEXT_H__
