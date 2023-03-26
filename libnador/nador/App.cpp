#include <sstream>

#include "nador/App.h"
#include "nador/utils/Time.h"
#include "nador/common/GlobalEvents.h"
#include "nador/video/renderer/Renderer.h"

#include "nador/system/Factory.h"
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

namespace nador
{
    //----------------------------------------------------------------------------------------------

    IApp* IApp::s_instance = nullptr;

    IApp::IApp()
    {
        if (s_instance)
        {
            ENGINE_FATAL("Application is already created and initialized!");
        }

        s_instance = this;
        ENGINE_DEBUG("App instance created.");
    }

    IApp::~IApp()
    {
        s_instance = nullptr;
        ENGINE_DEBUG("App instance destroyed.");
    }

    IApp* IApp::Get()
    {
        if (s_instance == nullptr)
        {
            ENGINE_FATAL("Application is not created yet!");
        }

        return s_instance;
    }

    //----------------------------------------------------------------------------------------------

    App::App(const AppConfig& config)
    : onWindowClose_listener_t(&g_onWindowCloseEvent, std::bind(&App::_onWindowClose, this))
    {
        ENGINE_DEBUG("App initializing...");

        _config = config;

        _factory.reset(new Factory(_config));
        _renderer.reset(new Renderer(GetVideo()));
        _fontCtrl.reset(new FontController(GetVideo()->GetMaxTextureSize()));
        _testController.reset(new TestController);

        IFileController* fileCtrl        = GetFileController();
        DataPtr          atlasConfigData = fileCtrl->Read(_config.atlasConfigPath);
        _atlasCtrl.reset(new AtlasController(atlasConfigData));

        _uiApp.reset(new UiApp);

        ShowDebugWindow(_config.showDebugWindow);
        ShowDebugInfo(_config.showDebugInfo);

        ENGINE_DEBUG("App initialized.");
    }

    App::~App()
    {
        ENGINE_DEBUG("App deinitializing....");

        _testController.reset();
        _uiApp.reset();
        _atlasCtrl.reset();
        _fontCtrl.reset();
        _renderer.reset();
        _factory.reset();

        ENGINE_DEBUG("App deinitialized.");
    }

    bool App::ShouldClose() const
    {
        return _state == EAppState::CLOSED;
    }

    void App::_TickBegin()
    {
        GetRenderer()->Begin();
        _factory->GetInputController()->TickBegin();
        _factory->GetWindow()->TickBegin();
        _factory->GetSoundController()->TickBegin();

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
        g_onRenderEvent();

        // Render debug event
        g_onDebugRenderEvent();

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
        _factory->GetWindow()->TickEnd();

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
        return _factory->GetVideo();
    }

    TestController* App::GetTestController()
    {
        return _testController.get();
    }

    IRenderer* App::GetRenderer()
    {
        return _renderer.get();
    }

    IWindow* App::GetWindow()
    {
        return _factory->GetWindow();
    }

    void App::ShowDebugWindow(bool show)
    {
        _testController->Suspend(!show);

        _factory->GetWindow()->ShowDebugWindow(show);
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
        return _factory->GetFileController();
    }

    const IInputController* App::GetInputController() const
    {
        return _factory->GetInputController();
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
        return _factory->GetSoundController();
    }

    const AppConfig& App::GetAppConfig() const
    {
        return _config;
    }

    IUiApp* App::GetUiApp() const
    {
        return _uiApp.get();
    }
} // namespace nador