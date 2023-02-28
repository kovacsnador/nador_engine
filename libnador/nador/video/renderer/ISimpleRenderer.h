#ifndef __NADOR_I_SIMPLE_RENDERER_H__
#define __NADOR_I_SIMPLE_RENDERER_H__

#include "nador/utils/Types.h"

namespace nador
{
	class IMaterial;

	struct RenderData;

	class ISimpleRenderer
	{
	protected:
		virtual ~ISimpleRenderer() = default;

	public:
		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual bool Flush() = 0;

		virtual void Draw(const IMaterial* material, const RenderData& renderData, const glm::mat4& uMVP) = 0;

		uint32_t GetDrawCount() const noexcept
		{
			return _drawCount;
		}

	protected:
		uint32_t _drawCount{ 0 };
	};
}

#endif // !__NADOR_I_SIMPLE_RENDERER_H__
