#ifndef __NADOR_UI_IMAGE_H__
#define __NADOR_UI_IMAGE_H__

#include "EImageName.h"

#include "nador/ui/IUiElement.h"
#include "nador/video/material/Material.h"

namespace nador
{
	class IRenderer;
	
	class UiImage : public IUiElement
	{
	public:
		UiImage(const glm::ivec2& position,
				const glm::ivec2& size,
				video::EImageName imageName,
				UiAlignment aligner = {},
				IUiElement* parent = nullptr);

		UiImage(const glm::ivec2& position,
			const glm::ivec2& size,
			UiAlignment aligner = {},
			IUiElement* parent = nullptr);

		// Full screnn image
		UiImage(video::EImageName imageName, UiAlignment aligner = {EHorizontalAlignment::STRETCH, EVerticalAlignment::STRETCH}, IUiElement* parent = nullptr);

		void SetImage(video::EImageName imageName);
		void SetOpacity(float_t opacity) noexcept;

		video::EImageName GetImage() const noexcept { return _imageName; };

		bool IsRealImage() const noexcept;
		void MakeEmptyImage() noexcept;

	protected:
		void OnTick(IUiLogicState* uiLogicState, bool mouseOver) override;
		void OnRender(IRenderer* renderer, const quadVertices_t& vertices) override;

	private:
		video::EImageName	_imageName{ video::EImageName::IMAGE_NAME_COUNT };
		BatchMaterial		_material;
		RenderData			_renderData;
	};
	CREATE_PTR_TYPES(UiImage);
}

#endif // !__NADOR_UI_IMAGE_H__
