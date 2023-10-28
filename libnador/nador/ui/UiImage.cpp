#include "nador/ui/UiImage.h"
#include "nador/ui/UiCommon.h"
#include "nador/video/atlas/IAtlasController.h"
#include "nador/video/renderer/IRenderer.h"
#include "nador/ui/IUiLogicState.h"

namespace nador
{
    UiImage::UiImage(const glm::ivec2& position, const glm::ivec2& size, video::EImageName imageName, UiAlignment aligner, IUiElement* parent)
    : IUiElement(position, size, aligner, parent)
    , _imageName(imageName)
    {
    }

    UiImage::UiImage(const glm::ivec2& position, const glm::ivec2& size, UiAlignment aligner, IUiElement* parent)
    : IUiElement(position, size, aligner, parent)
    {
    }

    void UiImage::SetImage(video::EImageName imageName)
    {
        if (imageName != _imageName)
        {
            _imageName = imageName;
			
			// clear render data to force update
            _renderData.Clear();
        }
    }

    void UiImage::SetOpacity(float_t opacity) noexcept
    {
        _material.uColor.w = opacity;
    }

    bool UiImage::IsRealImage() const noexcept
    {
        return _imageName != video::EImageName::IMAGE_NAME_COUNT;
    }

    void UiImage::MakeEmptyImage() noexcept
    {
        _imageName = video::EImageName::IMAGE_NAME_COUNT;
    }

    void UiImage::OnTick(IUiLogicState* uiLogicState, [[maybe_unused]] bool mouseOver)
	{
		if(IsRealImage() && _renderData.Empty())
		{
			auto atlasCtrl = uiLogicState->GetAtlasCtrl();
			ImageData imageData = atlasCtrl->GetImage(_imageName);

            _material.texture = imageData.texture;
            _renderData.SetSquareIndices();
            _renderData.SetTextureCoords(imageData.image->uvs);
		}
	}
    
	void UiImage::OnRender(IRenderer* renderer, const quadVertices_t& vertices)
    {
        if (IsRealImage())
        {
            _renderData.SetVertices(vertices);

            glm::mat4 modelMtx = CreateModelMtxWithScale(vertices, _scale);

            renderer->Draw(&_material, _renderData, modelMtx);
        }
    }
} // namespace nador