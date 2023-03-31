#include "nador/video/shader/ShaderController.h"

#ifdef USE_OPENGL_3
#include "nador/video/opengl3/shaders/ShaderOpenGL3.h"
#endif // USE_OPENGL_3

#include "nador/log/Log.h"

namespace nador
{
	ShaderController::ShaderController(const IVideo* video)
	: _video(video)
	{
		NADOR_ASSERT(video);

		_Add(video, EShader::BASE, BaseVertexShader(), BaseFragmentShader());
		_Add(video, EShader::TEXTURE, TextureVertexShader(), TextureFragmentShader());
		_Add(video, EShader::FONT, FontVertexShader(), FontFragmentShader());
		_Add(video, EShader::ROUND_EDGE, RoundEdgeVertexShader(), RoundEdgeFragmentShader());
		_Add(video, EShader::BATCH, BatchQuadVertexShader(), BatchQuadFragmentShader(video->GetMaxTextureUnits()).c_str());
	}

	ShaderPtr ShaderController::Get(EShader type)
	{
		auto shaderPtr = _shaders.find(type);
		if (shaderPtr != _shaders.end())
		{
			return shaderPtr->second;
		}
		return nullptr;
	}

	void ShaderController::_Add(const IVideo* video, EShader type, const char* vertexShader, const char* fragmentShader)
	{
		_shaders[type] = std::make_shared<Shader>(video, type, vertexShader, fragmentShader);
	}

	const char* ShaderController::BaseVertexShader()
	{
#ifdef USE_OPENGL_3
		return ShaderOpenGL3::BaseVertexShader();
#else
		return nullptr;
#endif // USE_OPENGL_3
	}

	const char* ShaderController::BaseFragmentShader()
	{
#ifdef USE_OPENGL_3
		return ShaderOpenGL3::BaseFragmentShader();
#else
		return nullptr;
#endif // USE_OPENGL_3
	}

	const char* ShaderController::TextureVertexShader()
	{
#ifdef USE_OPENGL_3
		return ShaderOpenGL3::TextureVertexShader();
#else
		return nullptr;
#endif // USE_OPENGL_3
	}

	const char* ShaderController::TextureFragmentShader()
	{
#ifdef USE_OPENGL_3
		return ShaderOpenGL3::TextureFragmentShader();
#else
		return nullptr;
#endif // USE_OPENGL_3
	}

	const char* ShaderController::FontVertexShader()
	{
#ifdef USE_OPENGL_3
		return ShaderOpenGL3::FontVertexShader();
#else
		return nullptr;
#endif // USE_OPENGL_3
	}

	const char* ShaderController::FontFragmentShader()
	{
#ifdef USE_OPENGL_3
		return ShaderOpenGL3::FontFragmentShader();
#else
		return nullptr;
#endif // USE_OPENGL_3
	}

	const char* ShaderController::RoundEdgeVertexShader()
	{
#ifdef USE_OPENGL_3
		return ShaderOpenGL3::RoundEdgeVertexShader();
#else
		return nullptr;
#endif // USE_OPENGL_3
	}

	const char* ShaderController::RoundEdgeFragmentShader()
	{
#ifdef USE_OPENGL_3
		return ShaderOpenGL3::RoundEdgeFragmentShader();
#else
		return nullptr;
#endif // USE_OPENGL_3
	}

	const char* ShaderController::BatchQuadVertexShader()
	{
#ifdef USE_OPENGL_3
		return ShaderOpenGL3::BatchQuadVertexShader();
#else
		return nullptr;
#endif // USE_OPENGL_3
	}

	std::string ShaderController::BatchQuadFragmentShader(size_t textureSlots)
	{
#ifdef USE_OPENGL_3
		return ShaderOpenGL3::BatchQuadFragmentShader(textureSlots);
#else
		return nullptr;
#endif // USE_OPENGL_3
	}
}