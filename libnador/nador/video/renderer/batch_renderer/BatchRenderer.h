#ifndef __NADOR_BATCH_RENDERER__H__
#define __NADOR_BATCH_RENDERER__H__

#include <vector>

#include "nador/video/shader/Shader.h"
#include "nador/video/buffers/VertexBuffer.h"
#include "nador/video/buffers/Texture.h"
#include "nador/video/renderer/batch_renderer/BatchRenderData.h"
#include "nador/video/renderer/RenderData.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/IRenderPlugin.h"

namespace nador
{
	class IVideo;

	class BatchRenderer : public IRenderPlugin
	{
	public:
		BatchRenderer(const IVideoPtr video, ShaderPtr shader, size_t maxVertexCount, size_t maxTextureUnit);

		void Begin() override;
		void End() override;

		void Draw(const IMaterial* material, const RenderData& renderData, const glm::mat4& uMVP) override;

		bool Flush() override;

	private:
		void _ResetBuffers();
		size_t _AddTexture(TexturePtr texture);

		const IVideoPtr	_video;
		const size_t	_maxVertexCount;
		const size_t	_maxTextureUnit;

		VertexBufferPtr _vertexBuffer;
		TexturePtr		_whiteTexture;
		ShaderPtr		_shader;

		BatchRenderData			_batchRendererData;
		std::vector<TexturePtr>	_textureSlots;
		std::vector<int32_t>	_samplers;

		glm::mat4 _uMVP;
	};
	CREATE_PTR_TYPES(BatchRenderer);
}

#endif