#ifndef __SHADER_H__
#define __SHADER_H__

#include <memory>
#include <unordered_map>

#include "nador/video/IVideo.h"

namespace nador
{
	enum class EShader
	{
		BASE,
		TEXTURE,
		FONT,
		ROUND_EDGE,
		BATCH,
	};

	class Shader
	{
		using locationCache_t = std::unordered_map<std::string, int32_t>;
		using attribCache_t = std::vector<int32_t>;

	public:
		/*!
		 * Shader constructor.
		 *
		 * \param type				The shader type.
		 * \param vertexShader		The vertex shader source.
		 * \param fragmentShader	The fragment shader source.
		 */
		Shader(EShader type, const char* vertexShader, const char* fragmentShader);

		/*!
		 * Shader destructor.
		 */
		~Shader();

		/*!
		 * Bind the shader program.
		 */
		void Bind() const;

		/*!
		 * Unbind the shader program.
		 */
		void Unbind() const;

		/*!
		 * Enable attrib array in shader program.
		 *
		 * \param name			The attrib name.
		 * \param structSize	The size of the variable.
		 * \param offset		The data offset in attrib.
		 */
		void EnableAttribArray(const char* name, int32_t structSize, const void* offset, size_t stride = 0);

		/*!
		 * Disable all enabled attrib arrays in shader program.
		 */
		void DisableAttribArrays();

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name	The unifrom name.
		 * \param v0	The unifrom value.
		 */
		void SetUniform1i(const std::string& name, int32_t v0);
		void SetUniform1iv(const std::string& name, int32_t size, const int32_t* value);

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name	The unifrom name.
		 * \param v0	The unifrom value.
		 */
		void SetUniform1f(const std::string& name, float_t v0);
		void SetUniform1fv(const std::string& name, int32_t size, const float_t* value);

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name	The unifrom name.
		 * \param v0	The unifrom value.
		 * \param v1	The unifrom value.
		 * \param v2	The unifrom value.
		 * \param v3	The unifrom value.
		 */
		void SetUniform4f(const std::string& name, float_t v0, float_t v1, float_t v2, float_t v3);

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name		The unifrom name.
		 * \param vector	The vector to upload.
		 */
		void SetUniform4f(const std::string& name, const glm::vec4& vector);

		void SetUniform2f(const std::string& name, const glm::vec2& vec);

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name		The unifrom name.
		 * \param matrix	The unifrom value.
		 */
		void SetUniformMat4f(const std::string& name, const float_t* matrix);

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name		The unifrom name.
		 * \param matrix	The matrix to upload.
		 */
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

		/*!
		 * Gets the Shader type.
		 *
		 * \return The Shader type.
		 */
		EShader GetShaderType() const;

	private:
		/*!
		 * Gets the unifrom location id if exist.
		 *
		 * \param name		The unifrom name.
		 *
		 * \return	The unifrom id.
		 */
		int32_t _GetUniformLocation(const std::string& name);

		const IVideo*	_video;
		uint32_t		_id;
		EShader			_type;

		locationCache_t	_unifromCache;
		attribCache_t	_attribCache;
	};

	CREATE_PTR_TYPES(Shader);
}

#endif // !__SHADER_H__

