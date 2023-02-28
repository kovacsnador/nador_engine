#include "nador/system/Factory.h"
#include "nador/log/Log.h"

#ifdef USE_GLFW_WINDOW
#include "nador/system/window/WindowGLFW.h"
#include "nador/system/input/GLFWInputController.h"
#elif USE_SDL_WINDOW
#include "nador/system/window/WindowSDL.h"
#include "nador/system/input/SDLInputController.h"
#endif

#ifdef USE_OPENGL_3
#include "nador/video/opengl3/VideoOpenGL3.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "nador/system/file/WindowsFileController.h"
#endif

#include "nador/sound/open_al/OpenALSoundController.h"

#include "nador/system/IWindow.h"

namespace nador
{
	Factory::Factory(const AppConfig& config)
	: _video(nullptr)
	, _window(nullptr)
	, _fileController(nullptr)
	, _inputController(nullptr)
	, _soundController(nullptr)
	{
		_InitWindow(config);
		_InitVideo();
		_InitFileController(config.rootFilePath);
		_InitSoundController();
		_InitInputController();

		// Need to be inited after input controller
		_window->InitImGui();
	}

	Factory::~Factory()
	{
		delete _inputController;
		delete _video;
		delete _window;
		delete _fileController;
		delete _soundController;
	}

	IWindow* Factory::GetWindow() const
	{
		NADOR_ASSERT(_window);
		return _window;
	}

	const IWindow* Factory::GetWindowConst() const
	{
		NADOR_ASSERT(_window);
		return _window;
	}

	const IVideo* Factory::GetVideo() const
	{
		NADOR_ASSERT(_video);
		return _video;
	}

	IFileController* Factory::GetFileController() const
	{
		NADOR_ASSERT(_fileController);
		return _fileController;
	}

	IInputController* Factory::GetInputController() const
	{
		NADOR_ASSERT(_inputController);
		return _inputController;
	}

	ISoundController* Factory::GetSoundController() const
	{
		NADOR_ASSERT(_soundController);
		return _soundController;
	}

	void Factory::_InitInputController()
	{
#ifdef USE_GLFW_WINDOW
		_inputController = new GLFWInputController(_window->GetNativeApiWindow());
#elif USE_SDL_WINDOW
		_inputController = new SDLInputController(_window->GetNativeApiWindow());
#endif
	}

	void Factory::_InitWindow(const AppConfig& config)
	{
#ifdef USE_GLFW_WINDOW
		_window = new WindowGLFW();
#elif USE_SDL_WINDOW
		_window = new WindowSDL();
#endif

		if (config.windowDimension != glm::uvec2(0, 0))
		{
			// Default sized window
			_window->CreateWindow(
				config.windowDimension.x,
				config.windowDimension.y,
				config.appName.c_str(),
				config.vSync);
		}
		else
		{
			// Full Screen
			_window->CreateFullScreenWindow(config.appName.c_str(), config.vSync);
		}
	}

	void Factory::_InitVideo()
	{
#ifdef USE_OPENGL_3
		_video = new VideoOpenGL3();
#endif
	}

	void Factory::_InitFileController(const std::string_view& rootFilePath)
	{
#ifdef PLATFORM_WINDOWS
		_fileController = new WindowsFileController(rootFilePath);
#endif
	}

	void Factory::_InitSoundController()
	{
		_soundController = new OpenAlSoundContoller();
	}
}