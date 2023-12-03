#include "nador/ui/UiEditText.h"
#include "nador/ui/IUiApp.h"
#include "nador/video/renderer/IRenderer.h"
#include "nador/ui/IUiLogicState.h"

namespace nador
{
	UiEditText::UiEditText(const glm::ivec2& position, const glm::ivec2& size, UiEditTextImages images, const FontPtr& font, UiAlignment alignment, IUiElement* parent)
	: IUiElement(position, size, alignment, parent)
	, _images(images)
	, _uiImageBackground(position, size, images.idle, { EHorizontalAlignment::STRETCH, EVerticalAlignment::STRETCH }, this)
	, _bottomLine({0, 0}, { 0, DEFAULT_LINE_WIDTH }, { 1.0f, 1.0f, 1.0f, .6f }, { EHorizontalAlignment::STRETCH, EVerticalAlignment::BOTTOM }, this)
	, _cursor({ 0, 0 }, { DEFAULT_LINE_WIDTH, 0 }, { 1.0f, 1.0f, 1.0f, .6f }, { EHorizontalAlignment::LEFT, EVerticalAlignment::CENTER }, this)
	, _uiTextLabel({ 0, 0 }, font, "", {EHorizontalAlignment::LEFT, EVerticalAlignment::CENTER}, {20, 20, 1}, this)
	{
		SuspendInputEventsForChildrens(true);
	}

	UiEditText::UiEditText(const glm::ivec2& position, const glm::ivec2& size, const FontPtr& font, UiAlignment alignment, IUiElement* parent)
	: IUiElement(position, size, alignment, parent)
	, _uiImageBackground(position, size, { EHorizontalAlignment::STRETCH, EVerticalAlignment::STRETCH }, this)
	, _bottomLine({ 0, 0 }, { 0, DEFAULT_LINE_WIDTH }, { 1.0f, 1.0f, 1.0f, .6f }, { EHorizontalAlignment::STRETCH, EVerticalAlignment::BOTTOM }, this)
	, _cursor({ 0, 0 }, { DEFAULT_LINE_WIDTH, 0 }, { 1.0f, 1.0f, 1.0f, .6f }, { EHorizontalAlignment::LEFT, EVerticalAlignment::CENTER }, this)
	, _uiTextLabel({ 0, 0 }, font, "", { EHorizontalAlignment::LEFT, EVerticalAlignment::CENTER }, { 20, 20, 1 }, this)
	{
		SuspendInputEventsForChildrens(true);
	}

	UiImage& UiEditText::GetBackgroundImage()
	{
		return _uiImageBackground;
	}

	UiTextLabel& UiEditText::GetTextLabel()
	{
		return _uiTextLabel;
	}

	UiOverlay& UiEditText::GetBottomLine()
	{
		return _bottomLine;
	}

	UiOverlay& UiEditText::GetCursor()
	{
		return _cursor;
	}

	void UiEditText::SetCursorBlinkTime(float_t time)
	{
		_cursorBlinkTime = time;
	}

	void UiEditText::SetDefaultLineWidth(int32_t lineWidth)
	{
		_lineWidth = lineWidth;
		_UpdateLineWidth(lineWidth);
	}

	void UiEditText::OnMouseReleased(EMouseButton /*mouseButton*/, const glm::vec2& /*position*/)
	{
		_state = EState::EDIT;
	}

	bool UiEditText::OnCharPressed(const std::string& text)
	{
		if (_state == EState::EDIT)
		{
			_uiTextLabel.AppendChar(text);
			return true;
		}
		return false;
	}

	bool UiEditText::OnKeyPressed(EKeyCode keyCode)
	{
		if (_state == EState::EDIT)
		{
			switch (keyCode)
			{
			case EKeyCode::BACKSPACE:
				_uiTextLabel.RemoveLastChar();
				return true;
			default:
				break;
			}
		}

		return false;
	}

	bool UiEditText::OnKeyHolded(EKeyCode keyCode)
	{
		if (_state == EState::EDIT)
		{
			switch (keyCode)
			{
			case EKeyCode::BACKSPACE:
				_uiTextLabel.RemoveLastChar();
				return true;
			default:
				break;
			}
		}

		return false;
	}

	void UiEditText::OnTick(IUiLogicState* uiLogicState, bool mouseOver)
	{
		IUiApp* uiApp = uiLogicState->GetUiApp();

		if (mouseOver)
		{
			if (_state == EState::IDLE)
			{
				_state = EState::OVER;
			}
		}
		else if (uiApp->IsInFocus(this))
		{
			_state = EState::EDIT;
		}
		else
		{
			_state = EState::IDLE;
		}

		_UpdateTextSliding();

		_UpdateBackground(_state);
		_UpdateCursor(_state, uiLogicState->GetDeltaTime());
	}

	void UiEditText::OnRender(IRenderer* renderer, const quadVertices_t& vertices)
	{
		if (_CalculateTextLabelOffset() != 0)
		{
			renderer->Flush();

			renderer->SetScissor({ vertices[0].x, vertices[0].y }, GetSize());
		}
	}

	void UiEditText::OnRenderEnd(IRenderer* renderer)
	{
		if (_CalculateTextLabelOffset() != 0)
		{
			renderer->Flush();
			renderer->DisableScissor();
		}
	}

	void UiEditText::_UpdateBackground(EState state)
	{
		switch (state)
		{
		case EState::IDLE:
			_uiImageBackground.SetImage(_images.idle);
			break;
		case EState::EDIT:
			_uiImageBackground.SetImage(_images.edit);
			break;
		case EState::OVER:
			_uiImageBackground.SetImage(_images.over);
			break;
		default:
			_uiImageBackground.SetImage(_images.idle);
			break;
		}
	}

	void UiEditText::_UpdateCursor(EState state, float_t deltaTime)
	{
		if (state == EState::EDIT)
		{
			const glm::ivec2& size = GetSize();
			int32_t cursorX = _cursor.GetSize().x;

			int32_t textHeight = _uiTextLabel.GetTextHeight();
			int32_t textWidth = _uiTextLabel.GetTextWidth();

			int32_t cusrosPos = std::min(textWidth, size.x - cursorX);

			_cursor.SetSize({ cursorX, textHeight * 1.1f });
			_cursor.SetPosition({ cusrosPos, 0 });

			if (_lastCursorBlinking < _cursorBlinkTime)
			{
				_cursor.Show();

				_lastCursorBlinking += deltaTime;
			}
			else
			{
				_cursor.Hide();
				if (_lastCursorBlinking > _cursorBlinkTime * 2)
				{
					_lastCursorBlinking = 0.0f;
				}
				else
				{
					_lastCursorBlinking += deltaTime;
				}
			}
		}
		else
		{
			_cursor.Hide();
		}
	}

	void UiEditText::_UpdateLineWidth(int32_t lineWidth)
	{
		 _cursor.SetWidth(lineWidth);
		 _bottomLine.SetHeight(lineWidth);
	}

	void UiEditText::_UpdateTextSliding()
	{
		int32_t offset = _CalculateTextLabelOffset();
		_uiTextLabel.SetOffset({ offset, 0 });
	}

	int32_t UiEditText::_CalculateTextLabelOffset()
	{
		int32_t textWidth = _uiTextLabel.GetTextWidth();
		const glm::ivec2& size = GetSize();

		auto diff = textWidth - (size.x - _cursor.GetSize().x);

		if (diff > 0)
		{
			return -diff;
		}

		return 0;
	}
}