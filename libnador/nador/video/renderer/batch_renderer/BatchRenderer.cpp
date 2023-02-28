#include "nador/log/Log.h"
#include "nador/video/renderer/batch_renderer/BatchRenderer.h"
#include "nador/video/renderer/batch_renderer/BatchRenderData.h"
#include "nador/video/buffers/IndexBuffer.h"

namespace nador
{
	BatchRenderer::BatchRenderer(const IVideo* video, ShaderPtr shader, size_t maxVertexCount, size_t maxTextureUnit)
	: _video(video)
	, _maxVertexCount(maxVertexCount)
	, _maxTextureUnit(maxTextureUnit)
	, _shader(shader)
	, _uMVP(1.0f)
	{
		NADOR_ASSERT(_shader);
		NADOR_ASSERT(_video);

		uint32_t color = 0xffffffff;
		uint32_t width = 1;
		uint32_t height = 1;
		_whiteTexture.reset(new Texture(&color, width, height));

		_vertexBuffer.reset(new VertexBuffer(nullptr, sizeof(BatchVertex) * _maxVertexCount, VertexBuffer::EBufferType::DYNAMIC));
		
		_batchRendererData.vertices.resize(_maxVertexCount);
		_batchRendererData.indices.resize(_maxVertexCount);

		_textureSlots.push_back(_whiteTexture);

		// setup samplers
		for (size_t i = 0; i < _maxTextureUnit; ++i)
		{
			_samplers.push_back(i);
		}
	}

	void BatchRenderer::Draw(const IMaterial* material, const RenderData& renderData, const glm::mat4& uMVP)
	{
		NADOR_ASSERT(material);

		if (uMVP != _uMVP)
		{
			Flush();
			_uMVP = uMVP;
		}

		if (_batchRendererData.Size() + renderData.vertices.size() > _maxVertexCount)
		{
			// no place for vertices
			Flush();
		}

		const BatchMaterial* batchMaterial = dynamic_cast<const BatchMaterial*>(material);

		if (batchMaterial)
		{
			size_t slot = _AddTexture(batchMaterial->texture);

			BatchRenderData batchRenderData = CreateBatchRenderData(renderData, batchMaterial->uColor, static_cast<float_t>(slot), static_cast<float_t>(batchMaterial->IsText()));

			_batchRendererData.Add(batchRenderData);
		}
	}

	size_t BatchRenderer::_AddTexture(TexturePtr texture)
	{
		size_t currSize = _textureSlots.size();

		size_t slot = 0;

		if (texture)
		{
			for (; slot < currSize; ++slot)
			{
				if (_textureSlots.at(slot)->GetId() == texture->GetId())
				{
					// find in slots
					return slot;
				}
			}

			if (currSize + 1 >= _maxTextureUnit)
			{
				// no place for textures
				Flush();
			}

			slot = _textureSlots.size();
			_textureSlots.push_back(texture);
		}

		return slot;
	}

	void BatchRenderer::Begin()
	{
		_drawCount = 0;
	}

	void BatchRenderer::End()
	{
		Flush();
	}

	bool BatchRenderer::Flush()
	{
		if (_batchRendererData.vertices.empty())
		{
			return false;
		}

		auto vertexSize = sizeof(BatchVertex);
		auto floatSize = sizeof(float_t);

		_shader->Bind();

		_vertexBuffer->BufferSubData(0, utils::VectorsizeInBytes(_batchRendererData.vertices), &_batchRendererData.vertices.front());

		_shader->SetUniformMat4f("uMVP", _uMVP);

		_shader->EnableAttribArray("aPosition", sizeof(BatchVertex::position) / floatSize,		(const void*)offsetof(BatchVertex, position),		vertexSize);
		_shader->EnableAttribArray("aColor",	sizeof(BatchVertex::color) / floatSize,			(const void*)offsetof(BatchVertex, color),			vertexSize);
		_shader->EnableAttribArray("aTexCoord", sizeof(BatchVertex::textureCoords) / floatSize,	(const void*)offsetof(BatchVertex, textureCoords),	vertexSize);
		_shader->EnableAttribArray("aTexIndex", sizeof(BatchVertex::textureId) / floatSize,		(const void*)offsetof(BatchVertex, textureId),		vertexSize);
		_shader->EnableAttribArray("aIsText",	sizeof(BatchVertex::isText) / floatSize,		(const void*)offsetof(BatchVertex, isText),			vertexSize);

		// Bind textures
		_shader->SetUniform1iv("uTextures", _samplers.size(), &_samplers.front());
		
		for (size_t slot = 0; slot < _textureSlots.size(); ++slot)
		{
			_textureSlots.at(slot)->Bind(slot);
		}

		IndexBuffer indices(&_batchRendererData.indices.front(), _batchRendererData.indices.size());
		indices.Bind();

		_video->DrawElements(EDrawMode::E_TRIANGLES, _batchRendererData.indices.size(), nullptr);

		_shader->DisableAttribArrays();

		_ResetBuffers();

		_drawCount++;

		return true;
	}

	void BatchRenderer::_ResetBuffers()
	{
		_batchRendererData.Clear();

		_textureSlots.clear();
		_textureSlots.push_back(_whiteTexture);
	}
}