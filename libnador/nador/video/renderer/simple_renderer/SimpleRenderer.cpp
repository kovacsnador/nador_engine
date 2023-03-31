#include "nador/video/renderer/simple_renderer/SimpleRenderer.h"
#include "nador/video/buffers/IndexBuffer.h"

namespace nador
{
	SimpleRenderer::SimpleRenderer(const IVideo* video, IShaderController* shaderCtrl, size_t maxVertexCount)
	: _video(video)
	, _shaderCtrl(shaderCtrl)
	, _uMVP(1.0f)
	, _maxVertexCount(maxVertexCount)
	{
		NADOR_ASSERT(video);
		NADOR_ASSERT(shaderCtrl);
	}

	void SimpleRenderer::Begin()
	{
		_drawCount = 0;
	}

	void SimpleRenderer::End()
	{
		Flush();
	}

	void SimpleRenderer::Draw(const IMaterial* material, const RenderData& renderData, const glm::mat4& uMVP)
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

	bool SimpleRenderer::Flush()
	{
		if (_renderData.vertices.empty() || _renderData.indices.empty() || _currentMaterial == nullptr)
		{
			_ResetBuffers();
			return false;
		}

		_currentMaterial->Activate(_currentShader, _uMVP);

		VertexBuffer aPosition(_video, &_renderData.vertices.front(), utils::VectorsizeInBytes(_renderData.vertices));
		aPosition.Bind();

		decltype (_renderData.vertices)::value_type vertex;
		_currentShader->EnableAttribArray(_currentMaterial->GetVerticesName(), vertex.length(), nullptr);

		IndexBuffer indices(_video, &_renderData.indices.front(), _renderData.indices.size());
		indices.Bind();

		VertexBufferPtr aTexCoord;

		if (_renderData.texCoords.empty() == false)
		{
			aTexCoord = std::make_shared<VertexBuffer>(_video, &_renderData.texCoords.front(), utils::VectorsizeInBytes(_renderData.texCoords));
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

	bool SimpleRenderer::_IsSameMaterial(const IMaterial* material)
	{
		NADOR_ASSERT(material);

		if (_currentMaterial)
		{
			return _currentMaterial->IsSame(material);
		}

		return false;
	}

	bool SimpleRenderer::_SetMaterial(const IMaterial* material)
	{
		NADOR_ASSERT(material);

		_currentMaterial = material->Clone();
		_currentShader = _shaderCtrl->Get(_currentMaterial->ShaderTypeNeeded());

		NADOR_ASSERT(_currentMaterial);

		return true;
	}

	void SimpleRenderer::_AddRenderData(const RenderData& renderData)
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

	void SimpleRenderer::_ResetBuffers()
	{
		_renderData.Clear();
	}
}