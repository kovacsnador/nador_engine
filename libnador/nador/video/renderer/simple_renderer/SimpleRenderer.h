#ifndef __NADOR_SIMPLE_REDNERER_H__
#define __NADOR_SIMPLE_REDNERER_H__

#include "nador/video/renderer/ISimpleRenderer.h"
#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"
#include "nador/video/shader/ShaderController.h"

namespace nador
{
	class SimpleRenderer : public ISimpleRenderer
	{
	public:
		SimpleRenderer(const IVideo* video, IShaderController* shaderCtrl, size_t maxVertexCount);

		void Begin() override;
		void End() override;
		bool Flush() override;

		void Draw(const IMaterial* material, const RenderData& renderData, const glm::mat4& uMVP) override;

	private:
		bool _SetMaterial(const IMaterial* material);
		void _AddRenderData(const RenderData& renderData);
		void _ResetBuffers();

		bool _IsSameMaterial(const IMaterial* material);

		const IVideo*		_video;
		RenderData			_renderData;
		IMaterialPtr		_currentMaterial;
		ShaderPtr			_currentShader;
		IShaderController* _shaderCtrl;

		glm::mat4 _uMVP;

		size_t _maxVertexCount;
	};
	CREATE_PTR_TYPES(SimpleRenderer);
}

#endif // !__NADOR_SIMPLE_REDNERER_H__
