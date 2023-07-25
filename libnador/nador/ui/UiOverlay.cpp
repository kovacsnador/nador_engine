#include "nador/ui/UiOverlay.h"

namespace nador
{
	UiOverlay::UiOverlay(const glm::ivec2& position,
		const glm::ivec2& size,
		const glm::vec4& color,
		UiAlignment alignment,
		IUiElement* parent)
	: IUiElement(position, size, alignment, parent)
	{
		SetColor(color);

		_renderData.SetSquareIndices();
	}

	void UiOverlay::SetColor(const glm::vec4& color)
	{
		_material.uColor = color;
	}

	const glm::vec4& UiOverlay::GetColor() const noexcept
	{
		return _material.uColor;
	}

	void UiOverlay::OnRender(IRenderer* renderer, const quadVertices_t& vertices)
	{
		_renderData.SetVertices(vertices);

		glm::mat4 modelMtx = CreateModelMtxWithScale(vertices, _scale);

		_renderData.vertices *= modelMtx;

		renderer->Draw(&_material, _renderData, IDENTITY_MATRIX);
	}

	void UiOverlay::OnTick(IUiLogicState* uiLogicState)
	{
		// handle mouse over event
		if (IsOver(uiLogicState->GetMousePosition()) && uiLogicState->IsMouseOverHandled() == false)
		{
			uiLogicState->SetMouseOverHandled(true);
		}
	}

	//--------------------------------------------------------------------------------------------------------------

	UiRoundEdgeOverlay::UiRoundEdgeOverlay(const glm::ivec2& position, const glm::ivec2& size, const glm::vec4& color, UiAlignment aligner, IUiElement* parent)
	: IUiElement(position, size, aligner, parent)
	{
		SetColor(color);

		_renderData.SetSquareIndices();
		
		// need it for round the edge and calculate positions
		_renderData.texCoords = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f},
		};
	}

	void UiRoundEdgeOverlay::SetColor(const glm::vec4& color)
	{
		_material.uColor = color;
	}

	const glm::vec4& UiRoundEdgeOverlay::GetColor() const noexcept
	{
		return _material.uColor;
	}

	void UiRoundEdgeOverlay::SetEdgeRadius(float_t radius)
	{
		_material.uRadius = radius;
	}

	float_t UiRoundEdgeOverlay::GetEdgeRadius() const
	{
		return _material.uRadius;
	}

	void UiRoundEdgeOverlay::OnRender(IRenderer* renderer, const quadVertices_t& vertices)
	{
		_renderData.SetVertices(vertices);

		_material.uDimensions = GetSize();

		glm::mat4 modelMtx = CreateModelMtxWithScale(vertices, _scale);

		renderer->Draw(&_material, _renderData, modelMtx);
	}

	void UiRoundEdgeOverlay::OnTick(IUiLogicState* uiLogicState)
	{
		// handle mouse over event
		if (IsOver(uiLogicState->GetMousePosition()) && uiLogicState->IsMouseOverHandled() == false)
		{
			uiLogicState->SetMouseOverHandled(true);
		}
	}
}