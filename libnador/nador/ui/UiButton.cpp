#include "nador/ui/UiButton.h"

namespace nador
{
	UiButton::UiButton(const glm::ivec2& position, const glm::ivec2& size, UiButtonImages images, UiAlignment aligner, IUiElement* parent, bool isShow)
	: IUiElement(position, size, aligner, parent, isShow)
	, _uiImageBackground(position, size, images.idle, { EHorizontalAlignment::STRETCH, EVerticalAlignment::STRETCH }, this)
	, _images(images)
	{
		_uiImageBackground.SuspendInputEvents(true);
	}

	void UiButton::SetUiTextLabel(UiTextLabelPtr textLabel)
	{
		_uiTextLabelPtr = std::move(textLabel);

		_uiTextLabelPtr->SetAlignment({EHorizontalAlignment::CENTER, EVerticalAlignment::CENTER});
		_uiTextLabelPtr->SetPosition({0, 0});
		_uiTextLabelPtr->SetParent(this);

		_uiTextLabelPtr->SuspendInputEvents(true);
	}

	void UiButton::SetUiTextLabel(const FontPtr& font, std::string_view text)
	{
		_uiTextLabelPtr.reset(new UiTextLabel({0, 0}, font, text, {EHorizontalAlignment::CENTER, EVerticalAlignment::CENTER}, { 20, 20, 1 }, this));
		_uiTextLabelPtr->SuspendInputEvents(true);
	}

	void UiButton::SetScaleOnButtonDown(const glm::vec3& onDownScale)
	{
		_scaleOnButtonDown = onDownScale;
	}

	void UiButton::OnMousePressed(EMouseButton /*mouseButton*/, const glm::vec2& /*position*/)
	{
		_state = EState::DOWN;
	}

	void UiButton::OnMouseReleased(EMouseButton /*mouseButton*/, const glm::vec2& /*position*/)
	{
		_state = EState::IDLE;
	}

	void UiButton::OnTick(NADOR_MAYBE_UNUSED IUiLogicState* uiLogicState, bool mouseOver)
	{
		if (mouseOver)
		{	
			if (_state == EState::IDLE)
			{
				_state = EState::OVER;				
			}
		}
		else
		{
			_state = EState::IDLE;
		}

		glm::vec3 scale = { 1, 1, 1 };

		switch (_state)
		{
		case EState::IDLE:
			_uiImageBackground.SetImage(_images.idle);
			break;
		case EState::DOWN:
			_uiImageBackground.SetImage(_images.down);
			scale = _scaleOnButtonDown;
			break;
		case EState::OVER:
			_uiImageBackground.SetImage(_images.over);
			break;
		default:
			_uiImageBackground.SetImage(_images.idle);
			break;
		}

		SetScale(scale);
	}
}