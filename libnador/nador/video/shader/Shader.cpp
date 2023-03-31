#include "nador/video/shader/Shader.h"
#include "nador/log/Log.h"
#include "nador/App.h"

namespace nador
{
	Shader::Shader(const IVideo* video, EShader type, const char* vertexShader, const char* fragmentShader)
	: _video(video)
	, _id(0)
	, _type(type)
	{
		NADOR_ASSERT(_video);

		_id = _video->CreateShaderProgram(vertexShader, fragmentShader);

		ENGINE_DEBUG("Shader (EShader: %d) created with id %d", (int32_t)type, _id);
	}

	Shader::~Shader()
	{
		_video->DeleteShaderProgram(_id);
	}

	void Shader::Bind() const
	{
		_video->UseShaderProgram(_id);
	}

	void Shader::Unbind() const
	{
		_video->UseShaderProgram(0);
	}

	void Shader::EnableAttribArray(const char* name, int32_t structSize, const void* offset, size_t stride)
	{
		int32_t location = _video->GetAttribLocation(_id, name);
		if(location > -1)
		{
			_video->EnableVertexAttribArray(location, structSize, offset, stride);
			_attribCache.push_back(location);
		}
	}

	void Shader::DisableAttribArrays()
	{
		for(auto& it : _attribCache)
		{
			_video->DisableVertexAttribArray(it);
		}
		_attribCache.clear();
	}

	void Shader::SetUniform1i(const std::string& name, int32_t v0)
	{
		_video->SetUniform1i(_GetUniformLocation(name), v0);
	}

	void Shader::SetUniform1f(const std::string& name, float_t v0)
	{
		_video->SetUniform1f(_GetUniformLocation(name), v0);
	}

	void Shader::SetUniform1fv(const std::string& name, int32_t size, const float_t* value)
	{
		_video->SetUniform1fv(_GetUniformLocation(name), size, value);
	}

	void Shader::SetUniform1iv(const std::string& name, int32_t size, const int32_t* value)
	{
		_video->SetUniform1iv(_GetUniformLocation(name), size, value);
	}

	void Shader::SetUniform4f(const std::string& name, float_t v0, float_t v1, float_t v2, float_t v3)
	{
		_video->SetUniform4f(_GetUniformLocation(name), v0, v1, v2, v3);
	}

	void Shader::SetUniform4f(const std::string& name, const glm::vec4& vector)
	{
		_video->SetUniform4f(_GetUniformLocation(name), vector[0], vector[1], vector[2], vector[3]);
	}

	void Shader::SetUniform2f(const std::string& name, const glm::vec2& vec)
	{
		_video->SetUniform2f(_GetUniformLocation(name), vec.x, vec.y);
	}

	void Shader::SetUniformMat4f(const std::string& name, const float_t* matrix)
	{
		NADOR_ASSERT(matrix);
		_video->SetUniformMat4f(_GetUniformLocation(name), matrix);
	}

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		_video->SetUniformMat4f(_GetUniformLocation(name), &matrix[0][0]);
	}

	int32_t Shader::_GetUniformLocation(const std::string& name)
	{
		if(_unifromCache.find(name) != _unifromCache.end())
		{
			return _unifromCache[name];
		}

		int32_t location = _video->GetUniformLocation(_id, name.c_str());
		_unifromCache[name] = location;
		return location;
	}

	EShader Shader::GetShaderType() const
	{
		return _type;
	}
}