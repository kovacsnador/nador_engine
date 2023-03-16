#include "nador/ui/UiScrollList.h"
#include "nador/system/IInputController.h"

namespace nador
{
	UiScrollList::UiScrollList(const glm::ivec2& position, const glm::ivec2& size, UiAlignment alignment, IUiElement* parent)
	: IUiElement(position, size, alignment, parent)
	, _slider({0, 0}, {10, 50}, { 1.0f, 1.0f, 1.0f, 0.6f}, { EHorizontalAlignment::RIGHT, EVerticalAlignment::TOP}, this)
	{
		HandleInputEventBeforeChildren(true);
		SetDefaultMouseHandling(false);

		_slider.SetEdgeRadius(4);
		_slider.Hide();

		glm::vec2 scrollListSize = GetSize();

		_slider.SetSize({ 10, scrollListSize.y * 0.1f });
	}

	void UiScrollList::AddElement(IUiElementPtr element)
	{
		NADOR_ASSERT(element);

		_elements.push_back(element);
		AddChild(element.get());

		_aligner.AddElement(element.get());

		_slider.BringToFront();
	}

	void UiScrollList::OnRender(IRenderer* renderer, const quadVertices_t& vertices)
	{
		renderer->Flush();
	
		// takes the left bottom corner
		renderer->SetScissor({ vertices[0].x, vertices[0].y }, _size);
	}

	void UiScrollList::OnMousePressed(EMouseButton /*mouseButton*/, const glm::vec2& position)
	{
		_state = EState::SCROLLING;
		_lastMousePos = position;
	}

	void UiScrollList::OnMouseReleased(EMouseButton /*mouseButton*/, const glm::vec2& /*position*/)
	{
		_state = EState::IDLE;
	}

	void UiScrollList::OnRenderEnd(IRenderer* renderer)
	{
		renderer->Flush();
		renderer->DisableScissor();
	}

	void UiScrollList::OnTick(IUiLogicState* uiLogicState)
	{
		if (IsMouseOver(uiLogicState->GetInputCtrl()) && _state == EState::SCROLLING)
		{
			const IInputController* inputCtrl = uiLogicState->GetInputCtrl();
			glm::ivec2 currMousePos = inputCtrl->GetMousePosition();

			glm::vec2 offset = _lastMousePos - currMousePos;

			if (glm::abs(offset.x) > 1 || glm::abs(offset.y) > 1)
			{
				uiLogicState->SetMouseOverHandled(true);
				SuspendInputEventsForChildrens(true);

				glm::ivec2 maxOffset = _aligner.CalculateMaxOffset();
				_aligner.SetMinOffset({ -maxOffset.x, -maxOffset.y });
				_aligner.SetMaxOffset({ 0,0 });

				_UpdateSlider();
				_slider.Show();

				_spinner.lastOffset = offset;
				_spinner.lastDeltaTime = uiLogicState->GetDeltaTime();
			}

			_aligner.SetOffset(_aligner.GetOffset() + offset);

			_lastMousePos = currMousePos;
		}
		else
		{
			SuspendInputEventsForChildrens(false);
			_state = EState::IDLE;
			_slider.Hide();

			glm::vec2 spinningOffset = _spinner.Calculate(uiLogicState->GetDeltaTime());
			_aligner.SetOffset(_aligner.GetOffset() + spinningOffset);
		}
	}

	void UiScrollList::SetBackground(video::EImageName imageName)
	{
		if (_background == nullptr)
		{
			_background.reset(new UiImage({ 0, 0 }, { 0, 0 }, imageName, { EHorizontalAlignment::STRETCH, EVerticalAlignment::STRETCH }, this));
			_background->SuspendInputEvents(true);
		}
		else
		{
			_background->SetImage(imageName);
		}
	}
	
	void UiScrollList::SetSliderColor(const glm::vec4& color)
	{
		_slider.SetColor(color);
	}

	UiImage* UiScrollList::GetBackground()
	{
		return _background.get();
	}

	UiRoundEdgeOverlay& UiScrollList::GetSlider()
	{
		return _slider;
	}

	UiAligner& UiScrollList::GetAligner()
	{
		return _aligner;
	}

	void UiScrollList::_UpdateSlider()
	{
		glm::vec2 precentage = _aligner.CalculateOffsetPercentage();
		glm::vec2 size = GetSize() - _slider.GetSize();

		glm::ivec2 silderOffset(size * precentage);
		silderOffset.x = 0;

		_slider.SetOffset(silderOffset);
	}

	glm::ivec2 UiScrollList::Spinner::Calculate(float_t currdDeltaTime)
	{
		glm::vec2 result{ 0, 0 };

		if (lastDeltaTime > 0 && lastOffset != glm::vec2(0.0f, 0.0f))
		{
			glm::vec2 diff = lastOffset / lastDeltaTime;
			result = (diff * currdDeltaTime) * (1 - currdDeltaTime * deceleration);

			lastOffset = result;
			lastDeltaTime = currdDeltaTime;
		}
		return result;
	}
}