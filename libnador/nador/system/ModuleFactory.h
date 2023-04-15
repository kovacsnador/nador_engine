#ifndef __NADOR_MODULE_FACTORY_H__
#define __NADOR_MODULE_FACTORY_H__

#include "nador/common/AppConfig.h"

#include "nador/system/IWindow.h"
#include "nador/video/IVideo.h"
#include "nador/system/IFileController.h"
#include "nador/system/IInputController.h"
#include "nador/sound/ISoundController.h"
#include "nador/video/renderer/IRenderer.h"
#include "nador/video/font/IFontController.h"
#include "nador/video/atlas/IAtlasController.h"
#include "nador/ui/IUiApp.h"
#include "nador/test/ITestController.h"
#include "nador/system/window/IImGuiAdapter.h"

namespace nador
{
	namespace ModuleFactory
	{
		IWindowUPtr CreateWindow(const WindowSettings& config);
		IVideoUPtr CreateVideo();
		IFileControllerUPtr CreateFileController(std::string_view rootFilePath);
		IInputControllerUPtr CreateInputController(void* nativeWindow);
		IImguiAdapterUPtr CreateImGuiAdapter();
		ISoundControllerUPtr CreateSoundController(const IFileController* fileCtrl);
		IRendererUPtr CreateRenderer(const IVideo* video);
        IFontControllerUPtr CreateFontController(const IVideo* video, const IFileController* fileCtrl);
        IAtlasControllerUPtr CreateAtlasController(const IVideo* video, const IFileController* fileCtrl, const AtlasSettings& settings);
        IUiAppUPtr CreateUiApp(const IVideo* video, const IInputController* inputCtrl);
        ITestControllerUPtr CreateTestController();
	}
}

#endif // !__NADOR_MODULE_FACTORY_H__

