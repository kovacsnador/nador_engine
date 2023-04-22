#ifndef __NADOR_I_SHADER_CONTROLLER_H__
#define __NADOR_I_SHADER_CONTROLLER_H__

#include "nador/utils/Types.h"
#include "nador/video/shader/Shader.h"

namespace nador
{
	class IShaderController
	{
	protected:
		virtual ~IShaderController() = default;

	public:
		virtual ShaderPtr Get(EShader type) = 0;
		virtual void Add(EShader type, const char* vertexShader, const char* fragmentShader) = 0;
	};
	CREATE_PTR_TYPES(IShaderController);

}

#endif //!__NADOR_I_SHADER_CONTROLLER_H__