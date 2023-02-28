#ifndef __API_OPENGL_3_H__
#define __API_OPENGL_3_H__

#include <GLEW/include/GL/glew.h>

namespace nador
{
	namespace APIOpenGL3
	{
		void glGenBuffers(GLsizei n, GLuint* buffers);
		void glBindBuffer(GLenum target, GLuint buffer);
		void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
		void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
		void glDeleteBuffers(GLsizei n, const GLuint* buffers);
		
		void glGenTextures(GLsizei n, GLuint* textures);
		void glBindTexture(GLenum target, GLuint texture);
		void glTexParameteri(GLenum target, GLenum pname, GLint param);
		void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
		void glPixelStorei(GLenum pname, GLint param);
		void glGenerateMipmap(GLenum target);
		void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data);
		void glTexSubImage2D(GLenum target, GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data);
		void glActiveTexture(GLenum texture);
		void glDeleteTextures(GLsizei n, const GLuint* textures);

		GLuint glCreateProgram();
		GLuint glCreateShader(GLenum shaderType);
		void glAttachShader(GLuint program, GLuint shader);
		void glLinkProgram(GLuint program);
		void glValidateProgram(GLuint program);
		void glDeleteShader(GLuint shader);
		void glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
		void glCompileShader(GLuint shader);
		void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
		void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
		void glDeleteProgram(GLuint program);
		void glUseProgram(GLuint program);

		GLint glGetUniformLocation(GLuint program, const GLchar* name);
		void glUniform1i(GLint location, GLint v0);
		void glUniform1iv(GLint location, GLsizei count, const GLint* value);
		void glUniform1f(GLint location, GLfloat v0);
		void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		void glUniform2f(GLint location, GLfloat v0, GLfloat v1);
		void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		void glUniform1fv(GLint location, GLsizei count, const GLfloat* value);

		GLint glGetAttribLocation(GLuint program, const GLchar* name);
		void glEnableVertexAttribArray(GLuint index);
		void glDisableVertexAttribArray(GLuint index);
		void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

		void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
		void glClear(GLbitfield mask);

		void glEnable(GLenum cap);
		void glDisable(GLenum cap);
		void glBlendFunc(GLenum sfactor, GLenum dfactor);

		void glGetBooleanv(GLenum pname, GLboolean* params);
		void glGetFloatv(GLenum pname, GLfloat* params);
		void glGetIntegerv(GLenum pname, GLint* params);

		void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

		void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);

		void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
	};
}

#endif // !__API_OPENGL_3_H__

