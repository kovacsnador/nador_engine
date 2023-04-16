#include "nador/system/ModuleFactory.h"
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
#include "nador/video/renderer/Renderer.h"
#include "nador/video/font/FontController.h"
#include "nador/video/atlas/AtlasController.h"
#include "nador/ui/UiApp.h"
#include "nador/test/TestController.h"

namespace nador
{
    IWindowUPtr ModuleFactory::CreateWindow(const WindowSettings& config)
    {
#ifdef USE_GLFW_WINDOW
        auto window = std::make_unique<WindowGLFW>();
#elif USE_SDL_WINDOW
        auto window = std::make_unique<WindowSDL>();
#endif

		NADOR_ASSERT(window);

        if (config.windowDimension != glm::uvec2(0, 0))
        {
            // Default sized window
            window->CreateWindow(config.windowDimension.x, config.windowDimension.y, config.appName.c_str(), config.vSync);
        }
        else
        {
            // Full Screen
            window->CreateFullScreenWindow(config.appName.c_str(), config.vSync);
        }

		return window;
    }

	IVideoUPtr ModuleFactory::CreateVideo()
	{
#ifdef USE_OPENGL_3
        return std::make_unique<VideoOpenGL3>();
#endif
	}

	IFileControllerUPtr ModuleFactory::CreateFileController(std::string_view rootFilePath)
	{
#ifdef PLATFORM_WINDOWS
        return std::make_unique<WindowsFileController>(rootFilePath);
#endif
	}

	IInputControllerUPtr ModuleFactory::CreateInputController(void* nativeWindow)
	{
#ifdef USE_GLFW_WINDOW
    	return std::make_unique<GLFWInputController>(nativeWindow);
#elif USE_SDL_WINDOW
    	return std::make_unique<SDLInputController>(nativeWindow);
#endif
	}

    IImguiAdapterUPtr ModuleFactory::CreateImGuiAdapter()
    {
        return std::make_unique<ImguiAdapter>();
    }

    ISoundControllerUPtr ModuleFactory::CreateSoundController(const IFileControllerPtr fileCtrl)
	{
		return std::make_unique<OpenAlSoundContoller>(fileCtrl);
	}

    IRendererUPtr ModuleFactory::CreateRenderer(const IVideoPtr video)
    {
        return std::make_unique<Renderer>(video);
    }

    IFontControllerUPtr ModuleFactory::CreateFontController(const IVideoPtr video, const IFileControllerPtr fileCtrl)
    {
        return std::make_unique<FontController>(video, fileCtrl);
    }

    IAtlasControllerUPtr ModuleFactory::CreateAtlasController(const IVideoPtr video, const IFileControllerPtr fileCtrl, const AtlasSettings& settings)
    {
        return std::make_unique<AtlasController>(video, fileCtrl, settings);
    }

    IUiAppUPtr ModuleFactory::CreateUiApp(const IVideoPtr video, const IInputControllerPtr inputCtrl)
    {
        return std::make_unique<UiApp>(video, inputCtrl);
    }

    ITestControllerUPtr ModuleFactory::CreateTestController()
    {
        return std::make_unique<TestController>();
    }
} // namespace nador