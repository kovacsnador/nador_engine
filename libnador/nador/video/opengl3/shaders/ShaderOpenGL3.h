#ifndef __SHADER_OPENGL_3_H__
#define __SHADER_OPENGL_3_H__

#include <string>

namespace nador
{
	namespace ShaderOpenGL3
	{
		//==================================================
		const char* BaseVertexShader();
		const char* BaseFragmentShader();

		//==================================================
		const char* TextureVertexShader();
		const char* TextureFragmentShader();

		//==================================================
		const char* FontVertexShader();
		const char* FontFragmentShader();

		//==================================================
		const char* RoundEdgeVertexShader();
		const char* RoundEdgeFragmentShader();

		//==================================================
		const char* BatchQuadVertexShader();
		std::string BatchQuadFragmentShader(size_t textureSlots);
	};
}

#endif // !__SHADER_OPENGL_3_H__

