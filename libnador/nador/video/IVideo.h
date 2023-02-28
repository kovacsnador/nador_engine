#ifndef __I_VIDEO_H__
#define __I_VIDEO_H__

#include "nador/utils/Types.h"
#include "nador/video/buffers/TextureSettings.h"

namespace nador
{
	enum class EDrawMode
	{
		E_TRIANGLES,
		E_LINES,
	};

	class IVideo
	{
	public:
		/*!
		 * Virtual destructor.
		 */
		virtual ~IVideo() = default;

		/*!
		 * Uploads a vertex buffer in GPU.
		 *
		 * \param data The vertex data.
		 * \param size The vertex data size.
		 *
		 * \return The renderer id.
		 */
		virtual uint32_t UploadVertexBuffer(const void* data, size_t size) const = 0;

		/*!
		 * Uploads a vertex buffer in GPU with dynamic bound.
		 *
		 * \param data The vertex data.
		 * \param size The vertex data size.
		 *
		 * \return The renderer id.
		 */
		virtual uint32_t UploadVertexBufferDynamic(const void* data, size_t size) const = 0;
		
		/*!
		 * Binds a vertex buffer in GPU.
		 *
		 * \param id The renderer id.
		 */
		virtual void BindVertexBuffer(uint32_t id) const = 0;

		virtual void BufferSubData(uint32_t location, size_t size, const void* data) const = 0;
		
		/*!
		 * Unbinds a vertex buffer in GPU.
		 */
		virtual void UnbindVertexBuffer() const = 0;

		/*!
		 * Deletes a vertex buffer in GPU.
		 *
		 * \param id The renderer id.
		 */
		virtual void DeleteVertexBuffer(uint32_t* id) const = 0;

		/*!
		 * Uploads an index buffer in GPU.
		 *
		 * \param data	The index data.
		 * \param count The count of the index.
		 *
		 * \return The renderer id.
		 */
		virtual uint32_t UploadIndexBuffer(const uint32_t* data, size_t count) const = 0;

		/*!
		 * Uploads an index buffer in GPU with dynamic bound.
		 *
		 * \param data	The index data.
		 * \param count The count of the index.
		 *
		 * \return The renderer id.
		 */
		virtual uint32_t UploadIndexBufferDynamic(const uint32_t* data, size_t count) const = 0;

		/*!
		 * Binds an index buffer in GPU.
		 *
		 * \param id The renderer id.
		 */
		virtual void BindIndexBuffer(uint32_t id) const = 0;
		
		/*!
		 * Unbinds an index buffer in GPU.
		 */
		virtual void UnbindIndexBuffer() const = 0;

		/*!
		 * Deletes an index buffer in GPU.
		 *
		 * \param id The renderer id.
		 */
		virtual void DeleteIndexBuffer(uint32_t* id) const = 0;

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
		virtual uint32_t UploadTexture(const void* data, int32_t height, int32_t width, const TextureSettings& textureSettings) const = 0;

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
		virtual void UploadSubTexture(const void* data, int32_t xOffset, int32_t yOffset, int32_t width, int32_t height, const TextureSettings& textureSettings) const = 0;

		/*!
		 * Binds a texture into the GPU.
		 *
		 * \param id	The renderer id.
		 * \param slot	The slot count.
		 */
		virtual void BindTexture(uint32_t id, uint32_t slot = 0) const = 0;

		/*!
		 * Unbinds a texture in the GPU.
		 */
		virtual void UnbindTexture() const = 0;

		/*!
		 * Deletes a texture from the GPU.
		 *
		 * \param id The renderer id pointer.
		 */
		virtual void DeleteTexture(uint32_t* id) const = 0;

		/*!
		 * Creates a shader program.
		 *
		 * \param vertexShader		The vertex shader source.
		 * \param fragmentShader	The fragment shader source.
		 *
		 * \return The shader program id.
		 */
		virtual uint32_t CreateShaderProgram(const char* vertexShader, const char* fragmentShader) const = 0;

		/*!
		 * Bindes a shader program in the GPU.
		 *
		 * \param id The renderer id pointer.
		 */
		virtual void UseShaderProgram(uint32_t id) const = 0;

		/*!
		 * Deletes a shader program from the GPU.
		 *
		 * \param id The renderer id.
		 */
		virtual void DeleteShaderProgram(uint32_t id) const = 0;

		/*!
		 * Located an uniform in shader program.
		 *
		 * \param id		The shader program id.
		 * \param uniform	The name of the uniform.

		 * \return The location id.
		 */
		virtual int32_t GetUniformLocation(uint32_t program, const char* uniform) const = 0;

		/*!
		 * Located an attrib location in shader program.
		 *
		 * \param id		The shader program id.
		 * \param uniform	The name of the attribute.

		 * \return The location id.
		 */
		virtual int32_t GetAttribLocation(uint32_t program, const char* attribName) const = 0;

		/*!
		 * Enable a vertex attrib array in shader program.
		 *
		 * \param id		The shader program id.
		 * \param size		The size of the variable.
		 * \param data		The data pointer.
		 */
		virtual void EnableVertexAttribArray(uint32_t id, int32_t size, const void* data, size_t stride = 0) const = 0;

		/*!
		 * Disable a vertex attrib array in shader program.
		 *
		 * \param id		The shader program id.
		 */
		virtual void DisableVertexAttribArray(uint32_t id) const = 0;

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name	The unifrom id.
		 * \param v0	The unifrom value.
		 */
		virtual void SetUniform1i(int32_t id, int32_t v0) const = 0;
		
		virtual void SetUniform1iv(int32_t id, int32_t size, const int32_t* value) const = 0;

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name	The unifrom id.
		 * \param v0	The unifrom value.
		 */
		virtual void SetUniform1f(int32_t id, float_t v0) const = 0;

		virtual void SetUniform1fv(int32_t id, int32_t size, const float_t* value) const = 0;
		
		/*!
		 * Sets the unifrom location.
		 *
		 * \param name	The unifrom id.
		 * \param v0	The unifrom value.
		 * \param v1	The unifrom value.
		 * \param v2	The unifrom value.
		 * \param v3	The unifrom value.
		 */
		virtual void SetUniform4f(int32_t id, float_t v0, float_t v1, float_t v2, float_t v3) const = 0;
		virtual void SetUniform2f(int32_t id, float_t v0, float_t v1) const = 0;

		/*!
		 * Sets the unifrom location.
		 *
		 * \param name		The unifrom id.
		 * \param matrix	The unifrom value.
		 */
		virtual void SetUniformMat4f(int32_t id, const float_t* matrix) const = 0;

		/*!
		 * Clears the rgba colors.
		 *
		 * \param r		The red channel.
		 * \param g		The green channel.
		 * \param b		The blue channel.
		 * \param a		The alpha channel.
		 */
		virtual void ClearColorRGBA(float_t r, float_t g, float_t b, float_t a) const = 0;

		/*!
		 * Enables the blending function.
		 */
		virtual void EnableBlend() const = 0;

		virtual void EnableDepthTest() const = 0;
		virtual void EnableMultiSample() const = 0;

		virtual void SetScissor(const glm::ivec2& position, const glm::ivec2& size) const = 0;
		virtual void DisableScissor() const = 0;

		/*!
		 * Gets the screen size.
		 *
		 * \param width		The screen width referenc.
		 * \param height	The screen height referenc.
		 */
		virtual void GetScreenSize(int32_t& width, int32_t& height) const = 0;

		/*!
		 * Gets the screen size.
		 *
		 * \return		The screen size. 
		 */
		virtual const glm::ivec2& GetScreenSize() const = 0;

		/*!
		 * Draw the elements.
		 *
		 * \param mode			The draw mode.
		 * \param count			The indices count.
		 * \param indices		The indices data.
		 */
		virtual void DrawElements(EDrawMode mode, size_t count, const uint32_t* indices) const = 0;

		/*!
		 * Gets the max texture size which could be uploaded into GPU.
		 *
		 * \return		The max texture size.
		 */
		virtual int32_t GetMaxTextureSize() const = 0;

		virtual size_t GetMaxTextureUnits() const = 0;
	};
}

#endif // !__I_VIDEO_H__

