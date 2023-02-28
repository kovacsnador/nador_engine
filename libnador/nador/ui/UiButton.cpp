#include "nador/ui/UiButton.h"
#include "nador/App.h"

namespace nador
{
	UiButton::UiButton(const glm::ivec2& position, const glm::ivec2& size, UiButtonImages images, UiAlignment aligner, IUiElement* parent, bool isShow)
	: IUiElement(position, size, aligner, parent, isShow)
	, _uiImageBackground(position, size, images.idle, { EHorizontalAlignment::STRETCH, EVerticalAlignment::STRETCH }, this)
	, _images(images)
	{
		_uiImageBackground.SuspendInputEvents(true);
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

	void UiButton::OnTick(IUiLogicState* uiLogicState)
	{
		if (IsMouseOver(uiLogicState->GetInputCtrl()) && uiLogicState->IsMouseOverHandled() == false)
		{
			uiLogicState->SetMouseOverHandled(true);
			
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