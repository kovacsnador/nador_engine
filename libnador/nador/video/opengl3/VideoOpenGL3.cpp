#include <malloc.h>

#include "nador/log/ILog.h"
#include "nador/video/opengl3/VideoOpenGL3.h"

namespace nador
{
	uint32_t VideoOpenGL3::UploadVertexBuffer(const void* data, size_t size) const
	{
		uint32_t id = 0;
		APIOpenGL3::glGenBuffers(1, &id);
		APIOpenGL3::glBindBuffer(GL_ARRAY_BUFFER, id);
		APIOpenGL3::glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		APIOpenGL3::glBindBuffer(GL_ARRAY_BUFFER, 0);
		return id;
	}

	uint32_t VideoOpenGL3::UploadVertexBufferDynamic(const void* data, size_t size) const
	{
		uint32_t id = 0;
		APIOpenGL3::glGenBuffers(1, &id);
		APIOpenGL3::glBindBuffer(GL_ARRAY_BUFFER, id);
		APIOpenGL3::glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		APIOpenGL3::glBindBuffer(GL_ARRAY_BUFFER, 0);
		return id;
	}

	void VideoOpenGL3::BindVertexBuffer(uint32_t id) const
	{
		APIOpenGL3::glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void VideoOpenGL3::BufferSubData(uint32_t location, size_t size, const void* data) const
	{
		APIOpenGL3::glBufferSubData(GL_ARRAY_BUFFER, location, size, data);
	}

	void VideoOpenGL3::UnbindVertexBuffer() const
	{
		APIOpenGL3::glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VideoOpenGL3::DeleteVertexBuffer(uint32_t* id) const
	{
		APIOpenGL3::glDeleteBuffers(1, id);
	}

	uint32_t VideoOpenGL3::UploadIndexBuffer(const uint32_t* data, size_t count) const
	{
		uint32_t id = 0;
		APIOpenGL3::glGenBuffers(1, &id);
		APIOpenGL3::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		APIOpenGL3::glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
		APIOpenGL3::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return id;
	}

	uint32_t VideoOpenGL3::UploadIndexBufferDynamic(const uint32_t* data, size_t count) const
	{
		uint32_t id = 0;
		APIOpenGL3::glGenBuffers(1, &id);
		APIOpenGL3::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		APIOpenGL3::glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW);
		APIOpenGL3::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return id;
	}

	void VideoOpenGL3::BindIndexBuffer(uint32_t id) const
	{
		APIOpenGL3::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void VideoOpenGL3::UnbindIndexBuffer() const
	{
		APIOpenGL3::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void VideoOpenGL3::DeleteIndexBuffer(uint32_t* id) const
	{
		APIOpenGL3::glDeleteBuffers(1, id);
	}

	static bool SetTexturePixelStore(PixelStore pixelStore)
	{
		// Pixel store
		switch(pixelStore)
		{
		case PixelStore::GL_PACK_ALIGNMENT_1:
			APIOpenGL3::glPixelStorei(GL_PACK_ALIGNMENT, 1);
			break;
		case PixelStore::GL_PACK_ALIGNMENT_2:
			APIOpenGL3::glPixelStorei(GL_PACK_ALIGNMENT, 2);
			break;
		case PixelStore::GL_PACK_ALIGNMENT_4:
			APIOpenGL3::glPixelStorei(GL_PACK_ALIGNMENT, 4);
			break;
		case PixelStore::GL_PACK_ALIGNMENT_8:
			APIOpenGL3::glPixelStorei(GL_PACK_ALIGNMENT, 8);
			break;
		case PixelStore::GL_UNPACK_ALIGNMENT_1:
			APIOpenGL3::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			break;
		case PixelStore::GL_UNPACK_ALIGNMENT_2:
			APIOpenGL3::glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			break;
		case PixelStore::GL_UNPACK_ALIGNMENT_4:
			APIOpenGL3::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			break;
		case PixelStore::GL_UNPACK_ALIGNMENT_8:
			APIOpenGL3::glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
			break;
		case PixelStore::DEFAULT_STORE:
			break;
		default:
			ENGINE_WARNING("Pixel storei not set");
			return false;
		}
		return true;
	}

	static bool SetTextureFilter(TextureFilter filter, GLenum textureType)
	{
		switch(filter)
		{
		case TextureFilter::LINEAR:
			APIOpenGL3::glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			APIOpenGL3::glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case TextureFilter::NEAREST:
			APIOpenGL3::glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			APIOpenGL3::glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case TextureFilter::MIPMAP_LINEAR:
			APIOpenGL3::glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			APIOpenGL3::glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case TextureFilter::MIPMAP_NEAREST:
			APIOpenGL3::glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			APIOpenGL3::glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		default:
			ENGINE_WARNING("Texture filtering is not set!");
			return false;
		}
		return true;
	}

	static bool SetTextureWrapping(TextureWrapping wrapping, GLenum textureType)
	{
		switch(wrapping)
		{
		case TextureWrapping::REPEAT:
			APIOpenGL3::glTexParameterf(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
			APIOpenGL3::glTexParameterf(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case TextureWrapping::MIRRORED_REPEAT:
			APIOpenGL3::glTexParameterf(textureType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			APIOpenGL3::glTexParameterf(textureType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
		case TextureWrapping::CLAMP_TO_EDGE:
			APIOpenGL3::glTexParameterf(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			APIOpenGL3::glTexParameterf(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		default:
			ENGINE_WARNING("Texture Wrapping is not set!");
			return false;
		}
		return true;
	}
	
	static GLenum GetTextureType(TextureType target)
	{
		//Texture type
		switch(target)
		{
		case TextureType::TEXTURE_2D:
			return GL_TEXTURE_2D;
		default:
			ENGINE_FATAL("Texture type %d not implemented!", (int)target);
			break;
		}
		return 0;
	}

	static bool GetColorFormats(ColorFormat format, GLint& internalFormat, GLenum& colorFormat)
	{
		//Color format
		switch(format)
		{
		case ColorFormat::RGBA:
			colorFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
			break;
		case ColorFormat::ALPHA:
			colorFormat = GL_ALPHA;
			internalFormat = GL_ALPHA;
			break;
		default:
			ENGINE_FATAL("Color format %d not implemented!", (int)format);
			return false;
		}
		return true;
	}

	uint32_t VideoOpenGL3::UploadTexture(const void* data, int32_t height, int32_t width, const TextureSettings& textureSettings) const
	{
		uint32_t id = 0;
		APIOpenGL3::glGenTextures(1, &id);

		GLenum textureType = GetTextureType(textureSettings.target);
		
		GLint internalFormat = 0;
		GLenum colorFormat = 0;
		GetColorFormats(textureSettings.colorFormat, internalFormat, colorFormat);

		APIOpenGL3::glBindTexture(textureType, id);
		APIOpenGL3::glTexImage2D(textureType, 0, internalFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, data);

		SetTexturePixelStore(textureSettings.pixelStore);
		SetTextureFilter(textureSettings.filter, textureType);
		SetTextureWrapping(textureSettings.wrapping, textureType);
		APIOpenGL3::glGenerateMipmap(textureType);

		APIOpenGL3::glBindTexture(textureType, 0);
		return id;
	}

	void VideoOpenGL3::UploadSubTexture(const void* data, int32_t xOffset, int32_t yOffset, int32_t width, int32_t height, const TextureSettings& textureSettings) const
	{
		GLenum textureType = GetTextureType(textureSettings.target);

		GLint internalFormat = 0;
		GLenum colorFormat = 0;
		GetColorFormats(textureSettings.colorFormat, internalFormat, colorFormat);

		APIOpenGL3::glTexSubImage2D(textureType, 0, xOffset, yOffset, width, height, colorFormat, GL_UNSIGNED_BYTE, data);
	}

	void VideoOpenGL3::BindTexture(uint32_t id, uint32_t slot) const
	{
		APIOpenGL3::glActiveTexture(GL_TEXTURE0 + slot);
		APIOpenGL3::glBindTexture(GL_TEXTURE_2D, id);
	}

	void VideoOpenGL3::UnbindTexture() const
	{
		APIOpenGL3::glBindTexture(GL_TEXTURE_2D, 0);
	}

	void VideoOpenGL3::DeleteTexture(uint32_t* id) const
	{
		APIOpenGL3::glDeleteTextures(1, id);
	}

	uint32_t VideoOpenGL3::CreateShaderProgram(const char* vertexShader, const char* fragmentShader) const
	{
		uint32_t program = APIOpenGL3::glCreateProgram();
		uint32_t vs = _CompileShader(vertexShader, GL_VERTEX_SHADER);
		uint32_t fs = _CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

		APIOpenGL3::glAttachShader(program, vs);
		APIOpenGL3::glAttachShader(program, fs);
		APIOpenGL3::glLinkProgram(program);
		APIOpenGL3::glValidateProgram(program);

		APIOpenGL3::glDeleteShader(vs);
		APIOpenGL3::glDeleteShader(fs);

		return program;
	}

	uint32_t VideoOpenGL3::_CompileShader(const char* source, GLenum shaderType) const
	{
		uint32_t id = APIOpenGL3::glCreateShader(shaderType);
		
		APIOpenGL3::glShaderSource(id, 1, &source, nullptr);
		APIOpenGL3::glCompileShader(id);

		int32_t result;
		APIOpenGL3::glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if(result == GL_FALSE)
		{
			int32_t lenght;
			APIOpenGL3::glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
			char* message = (char*)alloca(lenght * sizeof(char));
			APIOpenGL3::glGetShaderInfoLog(id, lenght, &lenght, message);
			APIOpenGL3::glDeleteShader(id);
			ENGINE_FATAL(message);
			return 0;
		}
		return id;
	}

	void VideoOpenGL3::UseShaderProgram(uint32_t id) const
	{
		APIOpenGL3::glUseProgram(id);
	}

	void VideoOpenGL3::DeleteShaderProgram(uint32_t id) const
	{
		APIOpenGL3::glDeleteProgram(id);
	}

	int32_t VideoOpenGL3::GetUniformLocation(uint32_t program, const char* uniform) const
	{
		int32_t location = APIOpenGL3::glGetUniformLocation(program, uniform);
		if(location == -1)
		{
			ENGINE_WARNING("Uniform location %s not found in shader id %d!", uniform, program);
		}
		return location;
	}

	int32_t VideoOpenGL3::GetAttribLocation(uint32_t program, const char* attribName) const
	{
		int32_t location = APIOpenGL3::glGetAttribLocation(program, attribName);
		if(location == -1)
		{
			ENGINE_WARNING("Attrib location %s not found in shader id %d!", attribName, program);
		}
		return location;
	}

	void VideoOpenGL3::EnableVertexAttribArray(uint32_t id, int32_t size, const void* data, size_t stride) const
	{
		APIOpenGL3::glEnableVertexAttribArray(id);
		APIOpenGL3::glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, stride, data);
	}

	void VideoOpenGL3::DisableVertexAttribArray(uint32_t id) const
	{
		APIOpenGL3::glDisableVertexAttribArray(id);
	}

	void VideoOpenGL3::SetUniform1i(int32_t id, int32_t v0) const
	{
		APIOpenGL3::glUniform1i(id, v0);
	}

	void VideoOpenGL3::SetUniform1iv(int32_t id, int32_t size, const int32_t* value) const
	{
		APIOpenGL3::glUniform1iv(id, size, value);
	}

	void VideoOpenGL3::SetUniform1fv(int32_t id, int32_t size, const float_t* value) const
	{
		APIOpenGL3::glUniform1fv(id, size, value);
	}

	void VideoOpenGL3::SetUniform1f(int32_t id, float_t v0) const
	{
		APIOpenGL3::glUniform1f(id, v0);
	}

	void VideoOpenGL3::SetUniform4f(int32_t id, float_t v0, float_t v1, float_t v2, float_t v3) const
	{
		APIOpenGL3::glUniform4f(id, v0, v1, v2, v3);
	}

	void VideoOpenGL3::SetUniform2f(int32_t id, float_t v0, float_t v1) const
	{
		APIOpenGL3::glUniform2f(id, v0, v1);
	}

	void VideoOpenGL3::SetUniformMat4f(int32_t id, const float_t* matrix) const
	{
		APIOpenGL3::glUniformMatrix4fv(id, 1, GL_FALSE, matrix);
	}

	void VideoOpenGL3::ClearColorRGBA(float_t r, float_t g, float_t b, float_t a) const
	{
		APIOpenGL3::glClearColor(r, g, b, a);
		APIOpenGL3::glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}

	void VideoOpenGL3::EnableBlend() const
	{
		APIOpenGL3::glEnable(GL_BLEND);
		APIOpenGL3::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void VideoOpenGL3::EnableDepthTest() const
	{
		APIOpenGL3::glEnable(GL_DEPTH_TEST);
	}

	void VideoOpenGL3::EnableMultiSample() const
	{
		APIOpenGL3::glEnable(GL_MULTISAMPLE);
	}

	void VideoOpenGL3::SetScissor(const glm::ivec2& position, const glm::ivec2& size) const
	{
		APIOpenGL3::glEnable(GL_SCISSOR_TEST);
		APIOpenGL3::glScissor(position.x, position.y, size.x, size.y);
	}

	void VideoOpenGL3::DisableScissor() const
	{
		APIOpenGL3::glDisable(GL_SCISSOR_TEST);
	}

	void VideoOpenGL3::GetScreenSize(int32_t& width, int32_t& height) const
	{
		GLint viewport[4];
		APIOpenGL3::glGetIntegerv(GL_VIEWPORT, viewport);
		width = viewport[2];
		height = viewport[3];
	}

	const glm::ivec2& VideoOpenGL3::GetScreenSize() const
	{
		static glm::ivec2 screenSize;
		GetScreenSize(screenSize.x, screenSize.y);
		return screenSize;
	}

	void VideoOpenGL3::DrawElements(EDrawMode mode, size_t count, const uint32_t* indices) const
	{
		GLenum glDrawMode = GL_TRIANGLES;

		switch (mode)
		{
		case nador::EDrawMode::E_TRIANGLES:
			glDrawMode = GL_TRIANGLES;
			break;
		case nador::EDrawMode::E_LINES:
			glDrawMode = GL_LINES;
			break;
		}

		APIOpenGL3::glDrawElements(glDrawMode, count, GL_UNSIGNED_INT, indices);
	}

	int32_t VideoOpenGL3::GetMaxTextureSize() const
	{
		int32_t maxTextureSize;
		APIOpenGL3::glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
		return maxTextureSize;
	}

	size_t VideoOpenGL3::GetMaxTextureUnits() const
	{
		int32_t maxTextureUnits;
		APIOpenGL3::glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
		return maxTextureUnits;
	}
}