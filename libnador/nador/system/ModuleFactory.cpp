#include "nador/system/ModuleFactory.h"
#include "nador/log/ILog.h"

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
#include "nador/system/file/DefaultFileController.h"
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

	IFileControllerUPtr ModuleFactory::CreateFileController(const std::filesystem::path& rootFilePath)
	{
#ifdef PLATFORM_WINDOWS
        return std::make_unique<DefaultFileController>(rootFilePath);
#endif
	}

	IInputControllerUPtr ModuleFactory::CreateInputController(void* nativeWindow, InputEventHandler handler)
	{
#ifdef USE_GLFW_WINDOW
    	return std::make_unique<GLFWInputController>(nativeWindow, std::move(handler));
#elif USE_SDL_WINDOW
    	return std::make_unique<SDLInputController>(nativeWindow, handler);
#endif
	}

    IImguiAdapterUPtr ModuleFactory::CreateImGuiAdapter()
    {
        return std::make_unique<ImguiAdapter>();
    }

    ISoundControllerUPtr ModuleFactory::CreateSoundController(const IFileControllerPtr& fileCtrl)
	{
		return std::make_unique<OpenAlSoundContoller>(fileCtrl);
	}

    IRendererUPtr ModuleFactory::CreateRenderer(const IVideoPtr& video, IRenderer::rendererPlugins_t& renderPlugins, std::unique_ptr<Camera> camera)
    {
        return std::make_unique<Renderer>(video, renderPlugins, std::move(camera));
    }

    IFontControllerUPtr ModuleFactory::CreateFontController(const font::FontTextureLoadStrategy_t<Texture>& loadStrategy, uint32_t maxTextureSize)
    {
        return std::make_unique<FontController>(loadStrategy, maxTextureSize);
    }

    IAtlasControllerUPtr ModuleFactory::CreateAtlasController(IAtlasController::AtlasList_t list, size_t cacheSize)
    {
        return std::make_unique<AtlasController>(std::move(list), cacheSize);
    }

    IUiAppUPtr ModuleFactory::CreateUiApp(const IVideoPtr& video, const IInputControllerPtr& inputCtrl, const IAtlasControllerPtr& atlasCtrl)
    {
        return std::make_unique<UiApp>(video, inputCtrl, atlasCtrl);
    }

    ITestControllerUPtr ModuleFactory::CreateTestController()
    {
        return std::make_unique<TestController>();
    }
} // namespace nador