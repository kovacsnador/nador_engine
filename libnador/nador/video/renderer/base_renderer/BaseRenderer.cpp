#include "nador/video/renderer/base_renderer/BaseRenderer.h"
#include "nador/video/buffers/IndexBuffer.h"

namespace nador
{
	BaseRenderer::BaseRenderer(const IVideoPtr video, IShaderControllerPtr shaderCtrl, size_t maxVertexCount)
	: _video(std::move(video))
	, _shaderCtrl(std::move(shaderCtrl))
	, _uMVP(1.0f)
	, _maxVertexCount(maxVertexCount)
	{
		NADOR_ASSERT(_video);
		NADOR_ASSERT(_shaderCtrl);
	}

	void BaseRenderer::Begin()
	{
		_drawCount = 0;
	}

	void BaseRenderer::End()
	{
		Flush();
	}

	void BaseRenderer::Draw(const IMaterial* material, const RenderData& renderData, const glm::mat4& uMVP)
	{
		bool sameMaterial = _IsSameMaterial(material);

		bool needFlush = false;

		needFlush |= !sameMaterial;
		needFlush |= (renderData.vertices.size() + _renderData.vertices.size() >= _maxVertexCount);
		needFlush |= (uMVP != _uMVP);

		if (needFlush)
		{
			Flush();
		}

		_uMVP = uMVP;

		if (sameMaterial == false)
		{	
			_SetMaterial(material);
		}

		_AddRenderData(renderData);
	}

	bool BaseRenderer::Flush()
	{
		if (_renderData.vertices.empty() || _renderData.indices.empty() || _currentMaterial == nullptr)
		{
			_ResetBuffers();
			return false;
		}

		_currentMaterial->Activate(_currentShader, _uMVP);

		VertexBuffer aPosition(_video.get(), &_renderData.vertices.front(), utils::VectorsizeInBytes(_renderData.vertices));
		aPosition.Bind();

		decltype (_renderData.vertices)::value_type vertex;
		_currentShader->EnableAttribArray(_currentMaterial->GetVerticesName(), vertex.length(), nullptr);

		IndexBuffer indices(_video.get(), &_renderData.indices.front(), _renderData.indices.size());
		indices.Bind();

		VertexBufferPtr aTexCoord;

		if (_renderData.texCoords.empty() == false)
		{
			aTexCoord = std::make_shared<VertexBuffer>(_video.get(), &_renderData.texCoords.front(), utils::VectorsizeInBytes(_renderData.texCoords));
			aTexCoord->Bind();

			decltype (_renderData.texCoords)::value_type texCoord;
			_currentShader->EnableAttribArray(_currentMaterial->GetTextureCoordName(), texCoord.length(), nullptr);
		}

		_video->DrawElements(_renderData.drawMode, _renderData.indices.size(), nullptr);
		_currentShader->DisableAttribArrays();

		_ResetBuffers();

		// counting up draw calls
		_drawCount++;

		return true;
	}

	bool BaseRenderer::_IsSameMaterial(const IMaterial* material)
	{
		NADOR_ASSERT(material);

		if (_currentMaterial)
		{
			return _currentMaterial->IsSame(material);
		}

		return false;
	}

	bool BaseRenderer::_SetMaterial(const IMaterial* material)
	{
		NADOR_ASSERT(material);

		_currentMaterial = material->Clone();
		_currentShader = _shaderCtrl->Get(_currentMaterial->ShaderTypeNeeded());

		NADOR_ASSERT(_currentMaterial);

		return true;
	}

	void BaseRenderer::_AddRenderData(const RenderData& renderData)
	{
		if (renderData.indices.empty() || renderData.vertices.empty())
		{
			return;
		}

		_renderData.drawMode = renderData.drawMode;
		_renderData.AddVertices(renderData.vertices);
		_renderData.AddTextureCoords(renderData.texCoords);

		ConcatIndices(_renderData.indices, renderData.indices);
	}

	void BaseRenderer::_ResetBuffers()
	{
		_renderData.Clear();
	}
}