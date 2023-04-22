#include "nador/video/shader/ShaderController.h"

#ifdef USE_OPENGL_3
#include "nador/video/opengl3/shaders/ShaderOpenGL3.h"
#endif // USE_OPENGL_3

#include "nador/log/Log.h"

namespace nador
{
	ShaderController::ShaderController(const IVideoPtr video)
	: _video(std::move(video))
	{
		NADOR_ASSERT(video);
				
		Add(EShader::BASE, BaseVertexShader(), BaseFragmentShader());
		Add(EShader::TEXTURE, TextureVertexShader(), TextureFragmentShader());
		Add(EShader::FONT, FontVertexShader(), FontFragmentShader());
		Add(EShader::ROUND_EDGE, RoundEdgeVertexShader(), RoundEdgeFragmentShader());
		Add(EShader::BATCH, BatchQuadVertexShader(), BatchQuadFragmentShader(video->GetMaxTextureUnits()).c_str());
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

	void ShaderController::Add(EShader type, const char* vertexShader, const char* fragmentShader)
	{
		_shaders[type] = std::make_shared<Shader>(_video.get(), type, vertexShader, fragmentShader);
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