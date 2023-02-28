#ifndef __VIDEO_OPENGL_3_H__
#define __VIDEO_OPENGL_3_H__

#include "APIOpenGL3.h"
#include "nador/video/IVideo.h"

namespace nador
{
	class VideoOpenGL3 : public IVideo
	{
	public:
		/*!
		 * Uploads a vertex buffer in GPU.
		 *
		 * \param data The vertex data.
		 * \param size The vertex data size.
		 *
		 * \return The renderer id.
		 */
		uint32_t UploadVertexBuffer(const void* data, size_t size) const override;

		/*!
		 * Uploads a vertex buffer in GPU with dynamic bound.
		 *
		 * \param data The vertex data.
		 * \param size The vertex data size.
		 *
		 * \return The renderer id.
		 */
		uint32_t UploadVertexBufferDynamic(const void* data, size_t size) const override;

		/*!
		 * Binds a vertex buffer in GPU.
		 *
		 * \param id The renderer id.
		 */
		void BindVertexBuffer(uint32_t id) const override;

		void BufferSubData(uint32_t location, size_t size, const void* data) const override;

		/*!
		 * Unbinds a vertex buffer in GPU.
		 */
		void UnbindVertexBuffer() const override;

		/*!
		 * Deletes a vertex buffer in GPU.
		 *
		 * \param id The renderer id pointer.
		 */
		void DeleteVertexBuffer(uint32_t* id) const override;

		/*!
		 * Uploads an index buffer in GPU.
		 *
		 * \param data The index data.
		 * \param count The count of the index.
		 *
		 * \return The renderer id.
		 */
		uint32_t UploadIndexBuffer(const uint32_t* data, size_t count) const override;

		/*!
		 * Uploads an index buffer in GPU with dynamic bound.
		 *
		 * \param data The index data.
		 * \param count The count of the index.
		 *
		 * \return The renderer id.
		 */
		uint32_t UploadIndexBufferDynamic(const uint32_t* data, size_t count) const override;

		/*!
		 * Binds an index buffer in GPU.
		 *
		 * \param id The renderer id.
		 */
		void BindIndexBuffer(uint32_t id) const override;

		/*!
		 * Unbinds an index buffer in GPU.
		 */
		void UnbindIndexBuffer() const override;

		/*!
		 * Deletes an index buffer in GPU.
		 *
		 * \param id The renderer id pointer.
		 */
		void DeleteIndexBuffer(uint32_t* id) const override;

		/*!
		 * Uploads a texture into the GPU.
		 *
		 * \param data				The texture data.
		 * \param height			The texture height.
		 * \param width				The texture width.
		 * \param textureSettings	The specified texture settings.
		 *
		 * \return The renderer id.
		 */
		uint32_t UploadTexture(const void* data, int32_t height, int32_t width, const TextureSettings& textureSettings) const override;

		/*!
		 * Uploads subimage of texture into the GPU.
		 *
		 * \param data				The texture data.
		 * \param xoffset			The x offset.
		 * \param yoffset			The y offset.
		 * \param width				The texture width.
		 * \param height			The texture height.
		 * \param textureSettings	The specified texture settings.
		 */
		void UploadSubTexture(const void* data, int32_t xOffset, int32_t yOffset, int32_t width, int32_t height, const TextureSettings& textureSettings) const override;

		/*!
		 * Binds a texture into the GPU.
		 *
		 * \param id The renderer id.
		 * \param slot The slot count.
		 */
		void BindTexture(uint32_t id, uint32_t slot = 0) const override;

		/*!
		 * Unbinds a texture in the GPU.
		 */
		void UnbindTexture() const override;

		/*!
		 * Deletes a texture from the GPU.
		 *
		 * \param id The renderer id pointer.
		 */
		void DeleteTexture(uint32_t* id) const override;

		/*!
		 * Creates a shader program.
		 *
		 * \param vertexShader		The vertex shader source.
		 * \param fragmentShader	The fragment shader source.
		 *
		 * \return The shader program id.
		 */
		uint32_t CreateShaderProgram(const char* vertexShader, const char* fragmentShader) const override;

		/*!
		 * Bindes a shader program in the GPU.
		 *
		 * \param id The renderer id pointer.
		 */
		void UseShaderProgram(uint32_t id) const override;

		/*!
		 * Deletes a shader program from the GPU.
		 *
		 * \param id The renderer id.
		 */
		void DeleteShaderProgram(uint32_t id) const override;

		/*!
		 * Located an uniform in shader program.
		 *
		 * \param program	The shader program id.
		 * \param uniform	The name of the uniform.

		 * \return The location id.
		 */
		int32_t GetUniformLocation(uint32_t program, const char* uniform) const override;

		/*!
		 * Located an attrib location in shader program.
		 *
		 * \param program	The shader program id.
		 * \param uniform	The name of the attribute.

		 * \return The location id.
		 */
		int32_t GetAttribLocation(uint32_t program, const char* attribName) const override;

		/*!
		 * Enable a vertex attrib array in shader program.
		 *
		 * \param id		The shader program id.
		 * \param size		The size of the variable.
		 * \param data		The data pointer.
		 */
		void EnableVertexAttribArray(uint32_t id, int32_t size, const void* data, size_t stride = 0) const override;

		/*!
		 * Disable a vertex attrib array in shader program.
		 *
		 * \param id		The shader program id.
		 */
		void DisableVertexAttribArray(uint32_t id) const override;

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name	The unifrom id.
		 * \param v0	The unifrom value.
		 */
		void SetUniform1i(int32_t id, int32_t v0) const override;

		void SetUniform1iv(int32_t id, int32_t size, const int32_t* value) const override;

		void SetUniform1fv(int32_t id, int32_t size, const float_t* value) const override;

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name	The unifrom id.
		 * \param v0	The unifrom value.
		 */
		void SetUniform1f(int32_t id, float_t v0) const override;

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name	The unifrom id.
		 * \param v0	The unifrom value.
		 * \param v1	The unifrom value.
		 * \param v2	The unifrom value.
		 * \param v3	The unifrom value.
		 */
		void SetUniform4f(int32_t id, float_t v0, float_t v1, float_t v2, float_t v3) const override;
		void SetUniform2f(int32_t id, float_t v0, float_t v1) const override;

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name		The unifrom id.
		 * \param matrix	The unifrom value.
		 */
		void SetUniformMat4f(int32_t id, const float_t* matrix) const override;

		/*!
		 * Clears the rgba colors.
		 *
		 * \param r		The red channel.
		 * \param g		The green channel.
		 * \param b		The blue channel.
		 * \param a		The alpha channel.
		 */
		void ClearColorRGBA(float_t r, float_t g, float_t b, float_t a) const override;

		/*!
		 * Enables the blending function.
		 */
		void EnableBlend() const override;

		void EnableDepthTest() const override;
		void EnableMultiSample() const override;

		void SetScissor(const glm::ivec2& position, const glm::ivec2& size) const override;
		void DisableScissor() const override;

		/*!
		 * Gets the screen size.
		 *
		 * \param width		The screen width referenc.
		 * \param height	The screen height referenc.
		 */
		void GetScreenSize(int32_t& width, int32_t& height) const override;

		/*!
		 * Gets the screen size.
		 *
		 * \return		The screen size.
		 */
		const glm::ivec2& GetScreenSize() const override;

		/*!
		 * Draw the elements.
		 *
		 * \param count			The indices count.
		 * \param indices		The indices data.
		 */
		void DrawElements(EDrawMode mode, size_t count, const uint32_t* indices) const override;

		/*!
		 * Gets the max texture size which could be uploaded into GPU.
		 *
		 * \return		The max texture size.
		 */
		int32_t GetMaxTextureSize() const override;

		size_t GetMaxTextureUnits() const override;

	private:
		/*!
		 * Compiles a shader program.
		 *
		 * \param source		The shader source.
		 * \param shaderType	The shader type.
		 *
		 * \return The shader id.
		 */
		uint32_t _CompileShader(const char* source, GLenum shaderType) const;
		
	};
}

#endif // !__VIDEO_OPENGL_3_H__

