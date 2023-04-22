#ifndef __SHADER_CONTROLLER_H__
#define __SHADER_CONTROLLER_H__

#include <unordered_map>

#include "nador/video/shader/IShaderController.h"


namespace nador
{
	class ShaderController : public IShaderController
	{
		using container_t = std::unordered_map<EShader, ShaderPtr>;

	public:
		/*!
		 * ShaderController constructor.
		 */
		ShaderController(const IVideoPtr video);

		/*!
		 * Gets the specific shader program.
		 *
		 * \param type	The shader type.
		 */
		 ShaderPtr Get(EShader type) override;

		 /*!
		 * Create a new shader program.
		 *
		 * \param type				The shader type.
		 * \param vertexShader		The vertex shader source.
		 * \param fragmentShader	The fragment shader source.
		 */
		void Add(EShader type, const char* vertexShader, const char* fragmentShader) override;

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

private:
		container_t 	_shaders;
		const IVideoPtr _video;
	};
}

#endif // !__SHADER_CONTROLLER_H__

