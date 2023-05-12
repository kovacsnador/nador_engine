#include <algorithm>

#include "nador/video/renderer/Renderer.h"
#include "nador/log/Log.h"

static constexpr size_t MAX_VERTEX_COUNT = 10000;

namespace nador
{
	Renderer::Renderer(const IVideoPtr video, rendererPlugins_t& renderers)
	: _video(std::move(video))
	, _attachedRenderers(std::move(renderers))
	, _cameraMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	, _projectionMatrix(1.0f)
	, _modelMatrix(1.0f)
	{
		NADOR_ASSERT(_video);

		if(_video)
		{
			int32_t width = 0;
			int32_t height = 0;
			_video->GetScreenSize(width, height);

			_projectionMatrix = glm::ortho(0.0f, (float_t)width, 0.0f, (float_t)height, -1.0f, 1.0f);
			//_projectionMatrix = glm::ortho(0.0f, (float_t)width, (float_t)height, 0.0f, -1.0f, 1.0f);

			_video->EnableBlend();
			_video->EnableMultiSample();
		}
	}

	void Renderer::Begin()
	{
		_video->ClearColorRGBA(0, 0, 0, 1);

		for(auto& [_, value] : _attachedRenderers)
		{
			value->Begin();
		}
	}

	void Renderer::Flush()
	{
		if (_currentActiveRenderer)
		{
			_currentActiveRenderer->Flush();
		}
	}

	void Renderer::End()
	{
		for(auto& [_, value] : _attachedRenderers)
		{
			value->End();
		}
	}

	void Renderer::Draw(const IMaterial* pMaterial,
						const RenderData& renderData,
						const glm::mat4* modelMatrix,
						const glm::mat4* projectionMatrix,
						const glm::mat4* cameraMatrix)
	{
		ERenderPlugin rendererPlugin = pMaterial->GetRenderPlugin();

		auto renderer = _attachedRenderers.find(rendererPlugin);

		if(renderer == _attachedRenderers.end())
		{
			ENGINE_ERROR("RenderPlugin not attached %d", rendererPlugin);
			return;
		}

		_SwitchRendererIfNecessary(renderer->second.get());

		if (modelMatrix)
		{
			SetModelMatrix(modelMatrix);
		}

		if (projectionMatrix)
		{
			SetProjectionMatrix(projectionMatrix);
		}

		if (cameraMatrix)
		{
			SetCameraMatrix(cameraMatrix);
		}

		glm::mat4 MVPmatrix = _projectionMatrix * _cameraMatrix * _modelMatrix;
		_currentActiveRenderer->Draw(pMaterial, renderData, MVPmatrix);

		// count up draw count
		CountUp();
	}

	void Renderer::SetCameraMatrix(const glm::mat4* cameraMatrix)
	{
		if (*cameraMatrix != _cameraMatrix)
		{
			_cameraMatrix = *cameraMatrix;
		}
	}

	void Renderer::SetProjectionMatrix(const glm::mat4* projectionMatrix)
	{
		if (*projectionMatrix != _projectionMatrix)
		{
			_projectionMatrix = *projectionMatrix;
		}
	}

	void Renderer::SetModelMatrix(const glm::mat4* modelMatrix)
	{
		if (*modelMatrix != _modelMatrix)
		{
			_modelMatrix = *modelMatrix;
		}
	}

	const glm::mat4& Renderer::GetCameraMatrix() const
	{
		return _cameraMatrix;
	}

	const glm::mat4& Renderer::GetProjectionMatrix() const
	{
		return _projectionMatrix;
	}

	const glm::mat4& Renderer::GetModelMatrix() const
	{
		return _modelMatrix;
	}

	const glm::ivec2& Renderer::GetScreenSize() const
	{
		return _video->GetScreenSize();
	}

	float_t Renderer::GetRenderPerInterval(float_t interval) const
	{
		return GetCountPerInterval(interval);
	}

	uint32_t Renderer::GetDrawCount() const noexcept
	{
		uint32_t drawCount = 0;
		for(auto& [key, value] : _attachedRenderers)
		{
			drawCount += value->GetDrawCount();
		}
		return drawCount;

	}

	void Renderer::SetScissor(const glm::ivec2& position, const glm::ivec2& size) const
	{
		_video->SetScissor(position, size);
	}

	void Renderer::DisableScissor() const
	{
		_video->DisableScissor();
	}

	void Renderer::_SwitchRendererIfNecessary(IRenderPlugin* nextRenderer)
	{
		NADOR_ASSERT(nextRenderer);

		if (_currentActiveRenderer != nextRenderer)
		{
			// switching renderer
			if (_currentActiveRenderer)
			{
				_currentActiveRenderer->Flush();
			}
			_currentActiveRenderer = nextRenderer;
		}
	}
}