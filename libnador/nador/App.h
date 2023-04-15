#ifndef __NADOR_APP_H__
#define __NADOR_APP_H__

#include "log/Log.h"
#include "utils/NonCopyable.h"

#include "common/GlobalEvents.h"
#include "test/ITestController.h"
#include "video/renderer/IRenderer.h"
#include "system/IFileController.h"
#include "system/IInputController.h"
#include "video/atlas/IAtlasController.h"
#include "video/font/IFontController.h"
#include "utils/TickTracker.h"
#include "sound/ISoundController.h"
#include "ui/IUiApp.h"
#include "common/AppConfig.h"
#include "IApp.h"
#include "system/ModuleFactory.h"

namespace nador
{
    class TestController;

    class App : public IApp, public onWindowClose_listener_t, private NonCopyable
    {
    public:
        static IAppUPtr CreateApp(const AppConfig& config);

        App(const AppConfig&     config,
            IWindowUPtr          window,
            IVideoUPtr           video,
            IFileControllerUPtr  fileCtrl,
            IInputControllerUPtr inputCtrl,
            ISoundControllerUPtr soundCtrl,
            IUiAppUPtr           uiApp,
            IRendererUPtr        renderer,
            IAtlasControllerUPtr atlasCtrl,
            IFontControllerUPtr  fontCtrl,
            ITestControllerUPtr  testController);

    public:
        virtual ~App();

        /*!
         * Ask API to close the window.
         *
         * \return True if the window should be closed otherwise False.
         */
        bool ShouldClose() const override;

        /*!
         * The Tick function.
         */
        void Tick() override;

        /*!
         * Run the application.
         * This function calls Tick in every frame.
         */
        void Run() override;

        /*!
         * Gets the video interface.
         *
         * \return The video interface.
         */
        const IVideo* GetVideo() override;

        /*!
         * Gets the TestController object.
         *
         * \return The test controller.
         */
        ITestController* GetTestController() override;

        /*!
         * Gets the Renderer object.
         *
         * \return The renderer.
         */
        IRenderer* GetRenderer() override;

        /*!
         * Gets the Window interface.
         *
         * \return The rwindow interface.
         */
        IWindow* GetWindow() override;

        /*!
         * Shows and hides the debug window.
         *
         * \param show	The flag.
         */
        void ShowDebugWindow(bool show) override;

        /*!
         * Sets to show the debug infos.
         *
         * \param show	The flag.
         */
        void ShowDebugInfo(bool show) override;

        /*!
         * Gets if shows the debug infos.
         *
         * \return	The flag.
         */
        bool IsShowDebugInfo() override;

        /*!
         * Gets the file controller.
         *
         * \return	The file controller interface.
         */
        IFileController* GetFileController() const override;

        /*!
         * Gets the input controller.
         *
         * \return	The file controller interface.
         */
        const IInputController* GetInputController() const override;

        /*!
         * Gets the atlas controller.
         *
         * \return	The atlas controller.
         */
        const IAtlasController* GetAtlasController() const override;

        /*!
         * Gets the font controller.
         *
         * \return	The font controller.
         */
        IFontController* GetFontController() override;

        /*!
         * Gets the app configuration.
         *
         * \return	The base configuration.
         */
        const AppConfig& GetAppConfig() const override;

        /*!
         * Gets the sound controller.
         *
         * \return	The sound controller interface.
         */
        ISoundController* GetSoundController() const override;

        /*!
         * Gets the ui application.
         *
         * \return	The ui application.
         */
        IUiApp* GetUiApp() const override;

        void InitializeDefaultTests() override;

    private:
        enum class EAppState
        {
            RUNNING,
            CLOSED,
        };

        /*!
         * The Tick begin function.
         */
        void _TickBegin();

        /*!
         * The Tick end function.
         */
        void _TickEnd();

        /*!
         * Renders some additional debug info.
         */
        void _RenderDebugInfo();

        /*!
         * Gets the debug info text.
         *
         * \return The debug info text.
         */
        const char* _GetDebugInfoText();

        void _onWindowClose();

        bool _showDebugInfo { false };

        AppConfig   _config;
        TickTracker _tickTracker;

        IWindowUPtr          _window { nullptr };
        IVideoUPtr           _video { nullptr };
        IFileControllerUPtr  _fileCtrl { nullptr };
        IInputControllerUPtr _inputCtrl { nullptr };
        ISoundControllerUPtr _soundCtrl { nullptr };
        IRendererUPtr        _renderer { nullptr };
        IAtlasControllerUPtr _atlasCtrl { nullptr };
        IFontControllerUPtr  _fontCtrl { nullptr };
        ITestControllerUPtr  _testCtrl { nullptr };
        IUiAppUPtr           _uiApp { nullptr };

        EAppState _state { EAppState::RUNNING };
    };
} // namespace nador

#endif // !__NADOR_APP_H__
