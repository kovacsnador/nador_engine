#include "APIOpenGL3.h"
#include "nador/log/ILog.h"

#ifdef NADOR_VIDEO_DEBUG
#define GL_CLEAR_ERRORS GLClearErrors(__FUNCTION__);
#define GL_CHECK_ERRORS GLCheckErrors();
#else
#define GL_CLEAR_ERRORS
#define GL_CHECK_ERRORS
#endif

namespace nador
{
	const char* ConvertGLErrorCode(GLenum err)
	{
		switch(err)
		{
		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:
			return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW:
			return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW:
			return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "GL_INVALID_FRAMEBUFFER_OPERATION";
		case GL_CONTEXT_LOST:
			return "GL_CONTEXT_LOST";
		case GL_TABLE_TOO_LARGE:
			return "GL_TABLE_TOO_LARGE";
		default:
			return "UNKNOW_OPENGL_ERROR";
		}
	}

	bool GLCheckErrors()
	{
		GLenum err;
		bool result = true;
		while((err = glGetError()) != GL_NO_ERROR)
		{
			ENGINE_FATAL("[OpenGL Error] %s : (%d)", ConvertGLErrorCode(err), err);
			result = false;
		}
		return result;
	}

	void GLClearErrors(const char* /*func*/)
	{	
		//ENGINE_DEBUG(func);
		GLCheckErrors();
	}

	namespace APIOpenGL3
	{
		void glGenBuffers(GLsizei n, GLuint* buffers)
		{
			GL_CLEAR_ERRORS
			::glGenBuffers(n, buffers);
			GL_CHECK_ERRORS
		}

		void glBindBuffer(GLenum target, GLuint buffer)
		{
			GL_CLEAR_ERRORS
			::glBindBuffer(target, buffer);
			GL_CHECK_ERRORS
		}

		void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
		{
			GL_CLEAR_ERRORS
			::glBufferData(target, size, data, usage);
			GL_CHECK_ERRORS
		}

		void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
		{
			GL_CLEAR_ERRORS
			::glBufferSubData(target, offset, size, data);
			GL_CHECK_ERRORS
		}

		void glDeleteBuffers(GLsizei n, const GLuint* buffers)
		{
			GL_CLEAR_ERRORS
			::glDeleteBuffers(n, buffers);
			GL_CHECK_ERRORS
		}

		void glGenTextures(GLsizei n, GLuint* textures)
		{
			GL_CLEAR_ERRORS
			::glGenTextures(n, textures);
			GL_CHECK_ERRORS
		}

		void glBindTexture(GLenum target, GLuint texture)
		{
			GL_CLEAR_ERRORS
			::glBindTexture(target, texture);
			GL_CHECK_ERRORS
		}

		void glTexParameteri(GLenum target, GLenum pname, GLint param)
		{
			GL_CLEAR_ERRORS
			::glTexParameteri(target, pname, param);
			GL_CHECK_ERRORS
		}

		void glTexParameterf(GLenum target, GLenum pname, GLfloat param)
		{
			GL_CLEAR_ERRORS
			::glTexParameterf(target, pname, param);
			GL_CHECK_ERRORS
		}

		void glPixelStorei(GLenum pname, GLint param)
		{
			GL_CLEAR_ERRORS
			::glPixelStorei(pname, param);
			GL_CHECK_ERRORS
		}

		void glGenerateMipmap(GLenum target)
		{
			GL_CLEAR_ERRORS
			::glGenerateMipmap(target);
			GL_CHECK_ERRORS
		}

		void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
		{
			GL_CLEAR_ERRORS
			::glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
			GL_CHECK_ERRORS
		}

		void glTexSubImage2D(GLenum target, GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data)
		{
			GL_CLEAR_ERRORS
			::glTexSubImage2D(target, level, xOffset, yOffset, width, height, format, type, data);
			GL_CHECK_ERRORS
		}

		void glActiveTexture(GLenum texture)
		{
			GL_CLEAR_ERRORS
			::glActiveTexture(texture);
			GL_CHECK_ERRORS
		}

		void glDeleteTextures(GLsizei n, const GLuint* textures)
		{
			GL_CLEAR_ERRORS
			::glDeleteTextures(n, textures);
			GL_CHECK_ERRORS
		}

		GLuint glCreateProgram()
		{
			GL_CLEAR_ERRORS
			GLuint id = ::glCreateProgram();
			GL_CHECK_ERRORS
			return id;
		}

		void glAttachShader(GLuint program, GLuint shader)
		{
			GL_CLEAR_ERRORS
			::glAttachShader(program, shader);
			GL_CHECK_ERRORS
		}

		void glLinkProgram(GLuint program)
		{
			GL_CLEAR_ERRORS
			::glLinkProgram(program);
			GL_CHECK_ERRORS
		}

		void glValidateProgram(GLuint program)
		{
			GL_CLEAR_ERRORS
			::glValidateProgram(program);
			GL_CHECK_ERRORS
		}

		void glDeleteShader(GLuint shader)
		{
			GL_CLEAR_ERRORS
			::glDeleteShader(shader);
			GL_CHECK_ERRORS
		}

		GLuint glCreateShader(GLenum shaderType)
		{
			GL_CLEAR_ERRORS
			GLuint id = ::glCreateShader(shaderType);
			GL_CHECK_ERRORS
			return id;
		}

		void glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length)
		{
			GL_CLEAR_ERRORS
			::glShaderSource(shader, count, string, length);
			GL_CHECK_ERRORS
		}

		void glCompileShader(GLuint shader)
		{
			GL_CLEAR_ERRORS
			::glCompileShader(shader);
			GL_CHECK_ERRORS
		}

		void glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
		{
			GL_CLEAR_ERRORS
			::glGetShaderiv(shader, pname, params);
			GL_CHECK_ERRORS
		}

		void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog)
		{
			GL_CLEAR_ERRORS
			::glGetShaderInfoLog(shader, maxLength, length, infoLog);
			GL_CHECK_ERRORS
		}

		void glDeleteProgram(GLuint program)
		{
			GL_CLEAR_ERRORS
			::glDeleteProgram(program);
			GL_CHECK_ERRORS
		}

		void glUseProgram(GLuint program)
		{
			GL_CLEAR_ERRORS
			::glUseProgram(program);
			GL_CHECK_ERRORS
		}

		void glUniform1i(GLint location, GLint v0)
		{
			GL_CLEAR_ERRORS
			::glUniform1i(location, v0);
			GL_CHECK_ERRORS
		}

		void glUniform1iv(GLint location, GLsizei count, const GLint* value)
		{
			GL_CLEAR_ERRORS
			::glUniform1iv(location, count, value);
			GL_CHECK_ERRORS
		}

		void glUniform1f(GLint location, GLfloat v0)
		{
			GL_CLEAR_ERRORS
			::glUniform1f(location, v0);
			GL_CHECK_ERRORS
		}

		void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
		{
			GL_CLEAR_ERRORS
			::glUniform4f(location, v0, v1, v2, v3);
			GL_CHECK_ERRORS
		}

		void glUniform2f(GLint location, GLfloat v0, GLfloat v1)
		{
			GL_CLEAR_ERRORS
			::glUniform2f(location, v0, v1);
			GL_CHECK_ERRORS
		}

		void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
		{
			GL_CLEAR_ERRORS
			::glUniformMatrix4fv(location, count, transpose, value);
			GL_CHECK_ERRORS
		}

		void glUniform1fv(GLint location, GLsizei count, const GLfloat* value)
		{
			GL_CLEAR_ERRORS
			::glUniform1fv(location, count, value);
			GL_CHECK_ERRORS
		}

		GLint glGetUniformLocation(GLuint program, const GLchar* name)
		{
			GL_CLEAR_ERRORS
			GLint id = ::glGetUniformLocation(program, name);
			GL_CHECK_ERRORS
			return id;
		}

		GLint glGetAttribLocation(GLuint program, const GLchar* name)
		{
			GL_CLEAR_ERRORS
			GLint id = ::glGetAttribLocation(program, name);
			GL_CHECK_ERRORS
			return id;
		}

		void glEnableVertexAttribArray(GLuint index)
		{
			GL_CLEAR_ERRORS
			::glEnableVertexAttribArray(index);
			GL_CHECK_ERRORS
		}

		void glDisableVertexAttribArray(GLuint index)
		{
			GL_CLEAR_ERRORS
			::glDisableVertexAttribArray(index);
			GL_CHECK_ERRORS
		}

		void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
		{
			GL_CLEAR_ERRORS
			::glVertexAttribPointer(index, size, type, normalized, stride, pointer);
			GL_CHECK_ERRORS
		}

		void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
		{
			GL_CLEAR_ERRORS
			::glClearColor(red, green, blue, alpha);
			GL_CHECK_ERRORS
		}

		void glClear(GLbitfield mask)
		{
			GL_CLEAR_ERRORS
			::glClear(mask);
			GL_CHECK_ERRORS
		}

		void glEnable(GLenum cap)
		{
			GL_CLEAR_ERRORS
			::glEnable(cap);
			GL_CHECK_ERRORS
		}

		void glDisable(GLenum cap)
		{
			GL_CLEAR_ERRORS
			::glDisable(cap);
			GL_CHECK_ERRORS
		}

		void glBlendFunc(GLenum sfactor, GLenum dfactor)
		{
			GL_CLEAR_ERRORS
			::glBlendFunc(sfactor, dfactor);
			GL_CHECK_ERRORS
		}

		void glGetBooleanv(GLenum pname, GLboolean* params)
		{
			GL_CLEAR_ERRORS
			::glGetBooleanv(pname, params);
			GL_CHECK_ERRORS
		}

		void glGetFloatv(GLenum pname, GLfloat* params)
		{
			GL_CLEAR_ERRORS
			::glGetFloatv(pname, params);
			GL_CHECK_ERRORS
		}

		void glGetIntegerv(GLenum pname, GLint* params)
		{
			GL_CLEAR_ERRORS
			::glGetIntegerv(pname, params);
			GL_CHECK_ERRORS
		}

		void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
		{
			GL_CLEAR_ERRORS
			::glDrawElements(mode, count, type, indices);
			GL_CHECK_ERRORS
		}

		void glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
		{
			GL_CLEAR_ERRORS
			::glScissor(x, y, width, height);
			GL_CHECK_ERRORS
		}

		void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
		{
			GL_CLEAR_ERRORS
			::glViewport(x, y, width, height);
			GL_CHECK_ERRORS
		}
	}
}