#ifndef __SHADER_CONTROLLER_H__
#define __SHADER_CONTROLLER_H__

#include <unordered_map>

#include "nador/video/shader/Shader.h"

namespace nador
{
	class IShaderController
	{
	protected:
		virtual ~IShaderController() = default;

	public:
		virtual ShaderPtr Get(EShader type) = 0;
	};

	class ShaderController : public IShaderController
	{
		using container_t = std::unordered_map<EShader, ShaderPtr>;

	public:
		/*!
		 * ShaderController constructor.
		 */
		ShaderController(size_t maxTextureUnit);

		/*!
		 * Gets the specific shader program.
		 *
		 * \param type	The shader type.
		 */
		 ShaderPtr Get(EShader type);

	private:
		/*!
		 * Gets the base vertex shader source.
		 *
		 * \return  The shader source.
		 */
		static const char* BaseVertexShader();

		/*!
		 * Gets the base fragment shader source.
		 *
		 * \return  The shader source.
		 */
		static const char* BaseFragmentShader();

		/*!
		 * Gets the Texture vertex shader source.
		 *
		 * \return  The shader source.
		 */
		static const char* TextureVertexShader();

		/*!
		 * Gets the Texture fragment shader source.
		 *
		 * \return  The shader source.
		 */
		static const char* TextureFragmentShader();

		/*!
		 * Gets the Font vertex shader source.
		 *
		 * \return  The shader source.
		 */
		static const char* FontVertexShader();

		/*!
		 * Gets the Font fragment shader source.
		 *
		 * \return  The shader source.
		 */
		static const char* FontFragmentShader();

		static const char* RoundEdgeVertexShader();
		static const char* RoundEdgeFragmentShader();

		static const char* BatchQuadVertexShader();
		static std::string BatchQuadFragmentShader(size_t textureSlots);

		/*!
		 * Create a new shader program.
		 *
		 * \param type				The shader type.
		 * \param vertexShader		The vertex shader source.
		 * \param fragmentShader	The fragment shader source.
		 */
		void _Add(EShader type, const char* vertexShader, const char* fragmentShader);

		container_t _shaders;
	};
}

#endif // !__SHADER_CONTROLLER_H__

