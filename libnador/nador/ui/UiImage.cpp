#include "nador/ui/UiImage.h"
#include "nador/ui/UiCommon.h"

namespace nador
{
	UiImage::UiImage(const glm::ivec2& position,
					 const glm::ivec2& size,
					 video::EImageName imageName,
					 UiAlignment aligner,
					 IUiElement* parent)
	: IUiElement(position, size, aligner, parent)
	, _imageName(imageName)
	{
		Update();
	}

	UiImage::UiImage(const glm::ivec2& position,
					 const glm::ivec2& size,
					 UiAlignment aligner,
					 IUiElement* parent)
	: IUiElement(position, size, aligner, parent)
	{
	}

	void UiImage::SetImage(video::EImageName imageName)
	{
		if (imageName != _imageName)
		{
			_imageName = imageName;
			Update();
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

	void UiImage::OnRender(IRenderer* renderer, const quadVertices_t& vertices)
	{
		if (IsRealImage())
		{ 
			_renderData.SetVertices(vertices);

			glm::mat4 modelMtx = CreateModelMtxWithScale(vertices, _scale);

			renderer->Draw(&_material, _renderData, &modelMtx);
		}
	}

	void UiImage::Update()
	{
		if (IsRealImage())
		{
			ImageData imageData = GetImageData(_imageName);

			_material.texture = imageData.texture;
			_renderData.SetSquareIndices();
			_renderData.SetTextureCoords(imageData.image->uvs);
		}
	}
}