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
        IWindowUPtr          CreateWindow(const WindowSettings& config);
        IVideoUPtr           CreateVideo();
        IFileControllerUPtr  CreateFileController(std::string_view rootFilePath);
        IInputControllerUPtr CreateInputController(void* nativeWindow);
        IImguiAdapterUPtr    CreateImGuiAdapter();
        ISoundControllerUPtr CreateSoundController(const IFileControllerPtr& fileCtrl);
        IRendererUPtr        CreateRenderer(const IVideoPtr& video, IRenderer::rendererPlugins_t& renderPlugins);
        IFontControllerUPtr  CreateFontController(const IVideoPtr& video, const IFileControllerPtr& fileCtrl);
        IAtlasControllerUPtr CreateAtlasController(const IVideoPtr& video, const IFileControllerPtr& fileCtrl, const AtlasSettings& settings);
        IUiAppUPtr           CreateUiApp(const IVideoPtr& video, const IInputControllerPtr& inputCtrl, const IAtlasControllerPtr& atlasCtrl);
        ITestControllerUPtr  CreateTestController();
    } // namespace ModuleFactory
} // namespace nador

#endif // !__NADOR_MODULE_FACTORY_H__
