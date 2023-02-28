#ifndef __UI_OVERLAY_H__
#define __UI_OVERLAY_H__

#include "nador/ui/IUiElement.h"
#include "nador/video/material/Material.h"

namespace nador
{
	class UiOverlay : public IUiElement
	{
	public:
		UiOverlay(const glm::ivec2& position,
				  const glm::ivec2& size,
				  const glm::vec4& color,
				  UiAlignment alignment = UiAlignment(),
				  IUiElement* parent = nullptr);

		void SetColor(const glm::vec4& color);
		const glm::vec4& GetColor() const noexcept;

	protected:
		void OnRender(IRenderer* renderer, const quadVertices_t& vertices) override;
		void OnTick(IUiLogicState* uiLogicState) override;

	private:
		BatchMaterial	_material;
		RenderData		_renderData;
	};
	CREATE_PTR_TYPES(UiOverlay);

	class UiRoundEdgeOverlay : public IUiElement
	{
	public:
		UiRoundEdgeOverlay(const glm::ivec2& position,
			const glm::ivec2& size,
			const glm::vec4& color,
			UiAlignment aligner = UiAlignment(),
			IUiElement* parent = nullptr);

		void SetColor(const glm::vec4& color);
		const glm::vec4& GetColor() const noexcept;

		void SetEdgeRadius(float_t radius);
		float_t GetEdgeRadius() const;

	protected:
		void OnRender(IRenderer* renderer, const quadVertices_t& vertices) override;
		void OnTick(IUiLogicState* uiLogicState) override;

	private:
		RoundEdgeMaterial	_material;
		RenderData			_renderData;
	};
	CREATE_PTR_TYPES(UiRoundEdgeOverlay);
}

#endif // !__UI_OVERLAY_H__
