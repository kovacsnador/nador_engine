#include <sstream>

#include "nador/App.h"
#include "nador/utils/Time.h"
#include "nador/common/GlobalEvents.h"
#include "nador/video/renderer/Renderer.h"

#include "nador/test/TestController.h"
#include "nador/video/renderer/Renderer.h"
#include "nador/system/IFileController.h"
#include "nador/system/IInputController.h"
#include "nador/video/atlas/AtlasController.h"
#include "nador/video/font/FontController.h"
#include "nador/utils/TickTracker.h"
#include "nador/sound/ISoundController.h"
#include "nador/ui/UiApp.h"
#include "nador/system/IWindow.h"

#include "nador/test/Tests.h"

namespace nador
{
    IAppUPtr App::CreateApp(const AppConfig& config)
    {
        IWindowUPtr          window    = ModuleFactory::CreateWindow(config.windowSettings);
        IVideoUPtr           video     = ModuleFactory::CreateVideo();
        IFileControllerUPtr  fileCtrl  = ModuleFactory::CreateFileController(config.rootFilePath);
        IInputControllerUPtr inputCtrl = ModuleFactory::CreateInputController(window->GetNativeApiWindow());

        // Attach after InputController created
        window->AttachImGuiAdapter(ModuleFactory::CreateImGuiAdapter());

        ISoundControllerUPtr soundCtrl = ModuleFactory::CreateSoundController(fileCtrl.get());

        IRendererUPtr        renderer  = ModuleFactory::CreateRenderer(video.get());
        IFontControllerUPtr  fontCtrl  = ModuleFactory::CreateFontController(video.get(), fileCtrl.get());
        IAtlasControllerUPtr atlasCtrl = ModuleFactory::CreateAtlasController(video.get(), fileCtrl.get(), config.atlasSettings);
        IUiAppUPtr           uiApp     = ModuleFactory::CreateUiApp(video.get(), inputCtrl.get());
        ITestControllerUPtr  testCtrl  = ModuleFactory::CreateTestController();

        return std::make_unique<App>(config,
                                     std::move(window),
                                     std::move(video),
                                     std::move(fileCtrl),
                                     std::move(inputCtrl),
                                     std::move(soundCtrl),
                                     std::move(uiApp),
                                     std::move(renderer),
                                     std::move(atlasCtrl),
                                     std::move(fontCtrl),
                                     std::move(testCtrl));
    }

    App::App(const AppConfig&     config,
             IWindowUPtr          window,
             IVideoUPtr           video,
             IFileControllerUPtr  fileCtrl,
             IInputControllerUPtr inputCtrl,
             ISoundControllerUPtr soundCtrl,
             IUiAppUPtr           uiApp,
             IRendererUPtr        renderer,
             IAtlasControllerUPtr atlasCtrl,
             IFontControllerUPtr  fontCtrl,
             ITestControllerUPtr  testCtrl)
    : onWindowClose_listener_t(&g_onWindowCloseEvent, std::bind(&App::_onWindowClose, this))
    , _config(config)
    , _window(std::move(window))
    , _video(std::move(video))
    , _fileCtrl(std::move(fileCtrl))
    , _inputCtrl(std::move(inputCtrl))
    , _soundCtrl(std::move(soundCtrl))
    , _renderer(std::move(renderer))
    , _atlasCtrl(std::move(atlasCtrl))
    , _fontCtrl(std::move(fontCtrl))
    , _testCtrl(std::move(testCtrl))
    , _uiApp(std::move(uiApp))
    {
        ShowDebugWindow(_config.windowSettings.showDebugWindow);
        ShowDebugInfo(_config.windowSettings.showDebugInfo);

        _testCtrl->SetToggleDebugTextCallback([this]() { ShowDebugInfo(!IsShowDebugInfo()); });

        ENGINE_DEBUG("App initialized.");
    }

    App::~App()
    {
        ENGINE_DEBUG("App deinitializing....");

        _testCtrl.reset();
        _uiApp.reset();
        _atlasCtrl.reset();
        _fontCtrl.reset();
        _renderer.reset();

        _inputCtrl.reset();
        _video.reset();
        _window.reset();
        _fileCtrl.reset();
        _soundCtrl.reset();

        ENGINE_DEBUG("App deinitialized.");
    }

    bool App::ShouldClose() const
    {
        return _state == EAppState::CLOSED;
    }

    void App::_TickBegin()
    {
        _renderer->Begin();
        _inputCtrl->TickBegin();
        _window->TickBegin();
        _soundCtrl->TickBegin();

        g_onTickBeginEvent();
    }

    void App::Tick()
    {
        // Begin
        _TickBegin();

        // Game Tick event
        float_t deltaTime = _tickTracker.GetLastDeltaTimeInSec();
        g_onTickEvent(deltaTime);

        // Render event
        g_onRenderEvent(GetRenderer());

        // Render debug event
        g_onDebugRenderEvent(GetRenderer());

        // End
        _TickEnd();
    }

    void App::Run()
    {
        // The application loop
        while (ShouldClose() == false)
        {
            Tick();
        }
    }

    void App::_TickEnd()
    {
        _RenderDebugInfo();

        _renderer->End();
        _window->TickEnd();

        g_onTickEndEvent();
    }

    void App::_RenderDebugInfo()
    {
        if (_showDebugInfo && !_fontCtrl->IsLoading())
        {
            const FontPtr defaultFont = _fontCtrl->GetDefaultSystemFont();

            if (defaultFont)
            {
                float_t fontSize = 17;

                BatchTextMaterial material;
                material.texture = defaultFont->GetTexture();
                material.uColor  = { 1.0f, .0f, .0f, 1.0f };

                const char* debugText = _GetDebugInfoText();

                RenderData renderData = defaultFont->CalculateUTF8Text(debugText);

                auto screenSize = _renderer->GetScreenSize();

                glm::mat4 projMultiCamera = _renderer->GetProjectionMatrix() * _renderer->GetCameraMatrix();
                glm::vec3 worldPosition   = utils::ScreenToWorldPosition({ 0, screenSize.y - fontSize }, _renderer->GetScreenSize(), projMultiCamera);

                worldPosition.z = 0;

                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), worldPosition);

                glm::mat4 scaleMatrix = glm::scale(glm::vec3(fontSize, fontSize, 1));

                modelMatrix *= scaleMatrix;

                VerticesMultiMatrix(renderData.vertices, modelMatrix);

                glm::mat4 identityMtx(1.0f);
                _renderer->Draw(&material, renderData, &identityMtx);
            }
        }
    }

    const char* App::_GetDebugInfoText()
    {
        static char debugText[256];

        snprintf(debugText,
                 sizeof(debugText),
                 "fps: %.0f\nrps: %.0f\ndpf: %d",
                 _tickTracker.GetFramePerSecond(),
                 _renderer->GetRenderPerInterval(1.f),
                 _renderer->GetDrawCount() + 1);

        return debugText;
    }

    void App::_onWindowClose()
    {
        _state = EAppState::CLOSED;
    }

    const IVideo* App::GetVideo()
    {
        return _video.get();
    }

    ITestController* App::GetTestController()
    {
        return _testCtrl.get();
    }

    IRenderer* App::GetRenderer()
    {
        return _renderer.get();
    }

    IWindow* App::GetWindow()
    {
        return _window.get();
    }

    void App::ShowDebugWindow(bool show)
    {
        _testCtrl->Suspend(!show);

        _window->ShowDebugWindow(show);
    }

    void App::ShowDebugInfo(bool show)
    {
        _showDebugInfo = show;
    }

    bool App::IsShowDebugInfo()
    {
        return _showDebugInfo;
    }

    IFileController* App::GetFileController() const
    {
        return _fileCtrl.get();
    }

    const IInputController* App::GetInputController() const
    {
        return _inputCtrl.get();
    }

    const IAtlasController* App::GetAtlasController() const
    {
        return _atlasCtrl.get();
    }

    IFontController* App::GetFontController()
    {
        return _fontCtrl.get();
    }

    ISoundController* App::GetSoundController() const
    {
        return _soundCtrl.get();
    }

    const AppConfig& App::GetAppConfig() const
    {
        return _config;
    }

    IUiApp* App::GetUiApp() const
    {
        return _uiApp.get();
    }

    void App::InitializeDefaultTests()
    {
        _testCtrl->AddTest<ClearColorTest>("Clear Color", GetVideo());
        _testCtrl->AddTest<BaseMaterialTest>("Base Material Test");
        _testCtrl->AddTest<RoundEdgeMaterialTest>("Round Edge Material Test");
        _testCtrl->AddTest<SimpleLineTest>("Simple Line Test");
        _testCtrl->AddTest<TextureTest>("Texture Test", GetVideo(), GetFileController());
        _testCtrl->AddTest<AtlasTest>("Atlas Test", GetAtlasController());
        _testCtrl->AddTest<FontTest>("Font Test", GetFontController());
        _testCtrl->AddTest<InputTest>("Input Test", GetInputController());
        _testCtrl->AddTest<SoundTest>("Sound Test", GetSoundController());
        _testCtrl->AddTest<UiSquareTest>("UiSquare Test", GetVideo(), GetFileController(), GetUiApp());
        _testCtrl->AddTest<UiElementsTest>("UiElements Test", GetUiApp(), GetFontController(), GetInputController());
    }
} // namespace nador