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
#include "nador/video/shader/ShaderController.h"
#include "nador/video/renderer/base_renderer/BaseRenderer.h"
#include "nador/video/renderer/batch_renderer/BatchRenderer.h"
#include "nador/common/Stopwatch.h"
#include "nador/test/Tests.h"
#include "nador/system/input/input_events/InputEvents.h"

namespace nador
{
    IAppUPtr App::CreateApp(const AppConfig& config)
    {
        nador::Stopwatch<std::chrono::system_clock> sw;

        IWindowPtr         window   = ModuleFactory::CreateWindow(config.windowSettings);
        IVideoPtr          video    = ModuleFactory::CreateVideo();
        IFileControllerPtr fileCtrl = ModuleFactory::CreateFileController(config.rootFilePath);

        // Create input handling and input controller
        InputEventHandler   InputEventHandler { nador::IsInputKeyHandledByOthers, nador::IsInputMouseHandledByOthers };
        IInputControllerPtr inputCtrl = ModuleFactory::CreateInputController(window->GetNativeApiWindow(), std::move(InputEventHandler));

        // Attach after InputController created
        window->AttachImGuiAdapter(ModuleFactory::CreateImGuiAdapter());

        // create sound controller
        ISoundControllerPtr soundCtrl = ModuleFactory::CreateSoundController();

        // Create renderer and plugins
        IShaderControllerPtr shaderCtrl = std::make_shared<ShaderController>(video);

        IRenderer::rendererPlugins_t rendererPlugins;
        rendererPlugins.insert(
            { ERenderPlugin::EBaseRenderer, std::make_unique<BaseRenderer>(video, shaderCtrl, config.videoSettings.maxVertexCount) });
        rendererPlugins.insert({ ERenderPlugin::EBatchRenderer,
                                 std::make_unique<BatchRenderer>(
                                     video, shaderCtrl->Get(EShader::BATCH), config.videoSettings.maxVertexCount, video->GetMaxTextureUnits()) });

        // Create Camera and renderer
        const auto&  windowSizes   = config.windowSettings.windowDimension;
        auto         projectionMtx = glm::ortho(0.0f, (float_t)windowSizes.x, 0.0f, (float_t)windowSizes.y, -1.0f, 1.0f);
        auto         orthoCamera   = std::make_unique<Camera>(projectionMtx, OrthograpicViewMtxCalculation);
        IRendererPtr renderer      = ModuleFactory::CreateRenderer(video, rendererPlugins, std::move(orthoCamera));

        // Create FontController
        auto fontTextureLoadStrategy = [video](uint32_t width, uint32_t height) {
            TextureSettings settings(TextureType::TEXTURE_2D,
                                     TextureFilter::LINEAR,
                                     TextureWrapping::CLAMP_TO_EDGE,
                                     ColorFormat::ALPHA,
                                     PixelStore::GL_UNPACK_ALIGNMENT_1);

            return std::make_unique<Texture>(video.get(), nullptr, width, height, settings);
        };

        IFontControllerPtr fontCtrl = ModuleFactory::CreateFontController(fontTextureLoadStrategy, video->GetMaxTextureSize());

        // Atlas controller
        const auto& atlasSettings   = config.atlasSettings;
        auto        atlasConfigData = fileCtrl->Read(atlasSettings.atlasConfigPath);
        auto        atlasConfigList = atlas::AtlasConfigParser::ParseAtlasConfigs(atlasConfigData);

        IAtlasController::AtlasList_t atlases;
        atlases.reserve(atlasConfigList.size());
        for (const auto& atlasCfg : atlasConfigList)
        {

            auto configData = fileCtrl->Read(atlasSettings.atlasImagesPath / atlasCfg.config);
            if (configData.has_value() == false)
            {
                throw atlas::AtlasParserException("Atlas config data could not be opened!");
            }

            auto imageConfigs = atlas::AtlasConfigParser::ParseAtlasImageConfigs<video::EImageName>(configData);

            std::vector<Image> imagesInAtlas;
            imagesInAtlas.reserve(imageConfigs.size());
            for (const auto& it : imageConfigs)
            {
                imagesInAtlas.emplace_back(it.width, it.height, it.name, it.atlasName, it.id, it.uvs);
            }

            auto textureLoaderStrategy = [video, fileCtrl](const std::filesystem::path& p) {
                auto textureData = fileCtrl->Read(p);
                if (textureData)
                {
                    return std::make_unique<Texture>(video.get(), textureData.value());
                }
                return std::unique_ptr<Texture> { nullptr };
            };

            atlases.emplace_back(std::make_shared<Atlas>(atlasSettings.atlasImagesPath / atlasCfg.image, imagesInAtlas, textureLoaderStrategy));
        }
        IAtlasControllerPtr atlasCtrl = ModuleFactory::CreateAtlasController(atlases, atlasSettings.atlasCacheSize);

        // Create UIApp
        IUiAppPtr uiApp = ModuleFactory::CreateUiApp(video, inputCtrl, atlasCtrl);

        // Create TestController
        ITestControllerPtr testCtrl = ModuleFactory::CreateTestController();

        auto app = std::make_unique<App>(config,
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

        ENGINE_DEBUG("Get total App creation time: %d ms", sw.Stop<std::chrono::milliseconds>().count());

        return app;
    }

    App::App(const AppConfig&    config,
             IWindowPtr          window,
             IVideoPtr           video,
             IFileControllerPtr  fileCtrl,
             IInputControllerPtr inputCtrl,
             ISoundControllerPtr soundCtrl,
             IUiAppPtr           uiApp,
             IRendererPtr        renderer,
             IAtlasControllerPtr atlasCtrl,
             IFontControllerPtr  fontCtrl,
             ITestControllerPtr  testCtrl)
    : onWindowClose_listener_t(g_onWindowCloseEvent, std::bind(&App::_onWindowClose, this))
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

        // need to delete before other modules (not holding refcounted Ptr to others)
        _testCtrl.reset();

        ENGINE_DEBUG("App deinitialized.");
    }

    bool App::ShouldClose() const noexcept
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
        g_onAnimationTickEvent(_tickTracker.DeltaTime<std::chrono::milliseconds>());
        g_onTickEvent(_tickTracker.GetLastDeltaTimeInSec());

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

                glm::vec3 worldPosition { 0, screenSize.y - fontSize, 0 };

                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), worldPosition);
                glm::mat4 scaleMatrix = glm::scale(glm::vec3(fontSize, fontSize, 1));

                modelMatrix *= scaleMatrix;

                renderData.vertices *= modelMatrix;

                _renderer->Draw(&material, renderData);
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
        _testCtrl->AddTest<AnimationTest>("Animation Test", GetUiApp());

        // adding windows
        auto renderer = GetRenderer();
        _testCtrl->AddWindow(CameraWindow("Camera Settings", renderer->GetCamera()));
    }
} // namespace nador