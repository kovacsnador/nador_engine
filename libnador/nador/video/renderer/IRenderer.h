#ifndef __NADOR_I_RENDERER_H__
#define __NADOR_I_RENDERER_H__

#include "nador/video/material/Material.h"
#include "nador/video/renderer/RenderData.h"

namespace nador
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

	public:
		virtual void Begin() = 0;
		virtual void Flush() = 0;
		virtual void End() = 0;


		virtual void Draw(const IMaterial* pMaterial,
						  const RenderData& renderData,
						  const glm::mat4* modelMatrix = nullptr,
						  const glm::mat4* projectionMatrix = nullptr,
						  const glm::mat4* cameraMatrix = nullptr) = 0;

		virtual const glm::mat4& GetCameraMatrix() const = 0;
		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetModelMatrix() const = 0;
		virtual const glm::ivec2& GetScreenSize() const = 0;

		virtual void SetScissor(const glm::ivec2& position, const glm::ivec2& size) const = 0;
		virtual void DisableScissor() const = 0;

		virtual float_t GetRenderPerInterval(float_t interval = 1.f) const = 0;

		virtual uint32_t GetDrawCount() const noexcept = 0;
	};
	CREATE_PTR_TYPES(IRenderer);
}

#endif // !__NADOR_I_RENDERER_H__
