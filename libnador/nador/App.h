#ifndef __NADOR_APP_H__
#define __NADOR_APP_H__

#include "log/Log.h"
#include "utils/NonCopyable.h"


#include "common/GlobalEvents.h"
#include "system/IFactory.h"
#include "test/TestController.h"
#include "video/renderer/IRenderer.h"
#include "system/IFileController.h"
#include "system/IInputController.h"
#include "video/atlas/IAtlasController.h"
#include "video/font/IFontController.h"
#include "utils/TickTracker.h"
#include "sound/ISoundController.h"
#include "ui/IUiApp.h"
#include "common/AppConfig.h"

namespace nador
{
	class IApp
	{
	protected:
		IApp();
		virtual ~IApp();
	
	public:
		static IApp* Get();

		virtual bool ShouldClose() const = 0;
		virtual void Tick() = 0;
		virtual void Run() = 0;
		virtual const IVideo* GetVideo() = 0;
		virtual TestController* GetTestController() = 0;
		virtual IRenderer* GetRenderer()  =0;
		virtual IWindow* GetWindow() = 0;
		virtual void ShowDebugWindow(bool show) = 0;
		virtual void ShowDebugInfo(bool show) = 0;
		virtual bool IsShowDebugInfo() = 0;
		virtual IFileController* GetFileController() const = 0;
		virtual const IInputController* GetInputController() const = 0;
		virtual const IAtlasController* GetAtlasController() const = 0;
		virtual IFontController* GetFontController() = 0;
		virtual const AppConfig& GetAppConfig() const = 0;
		virtual ISoundController* GetSoundController() const = 0;
		virtual IUiApp* GetUiApp() const = 0;

	protected:
		static IApp* s_instance;
	};

	class TestController;

	class App : public IApp, public onWindowClose_listener_t, private NonCopyable
	{
	public:
		App(const AppConfig& config);

		virtual ~App();

		/*!
		 * Ask API to close the window.
		 *
		 * \return True if the window should be closed otherwise False.
		 */
		bool ShouldClose() const;

		/*!
		 * The Tick function.
		 */
		void Tick();

		/*!
		 * Run the application.
		 * This function calls Tick in every frame.
		 */
		void Run();

		/*!
		 * Gets the video interface.
		 *
		 * \return The video interface.
		 */
		const IVideo* GetVideo();

		/*!
		 * Gets the TestController object.
		 *
		 * \return The test controller.
		 */
		TestController* GetTestController();

		/*!
		 * Gets the Renderer object.
		 *
		 * \return The renderer.
		 */
		IRenderer* GetRenderer();

		/*!
		 * Gets the Window interface.
		 *
		 * \return The rwindow interface.
		 */
		IWindow* GetWindow();

		/*!
		 * Shows and hides the debug window.
		 *
		 * \param show	The flag.
		 */
		void ShowDebugWindow(bool show);

		/*!
		 * Sets to show the debug infos.
		 *
		 * \param show	The flag.
		 */
		void ShowDebugInfo(bool show);

		/*!
		 * Gets if shows the debug infos.
		 *
		 * \return	The flag.
		 */
		bool IsShowDebugInfo();

		/*!
		 * Gets the file controller.
		 *
		 * \return	The file controller interface.
		 */
		IFileController* GetFileController() const;

		/*!
		 * Gets the input controller.
		 *
		 * \return	The file controller interface.
		 */
		const IInputController* GetInputController() const;

		/*!
		 * Gets the atlas controller.
		 *
		 * \return	The atlas controller.
		 */
		const IAtlasController* GetAtlasController() const;

		/*!
		 * Gets the font controller.
		 *
		 * \return	The font controller.
		 */
		IFontController* GetFontController();

		/*!
		 * Gets the app configuration.
		 *
		 * \return	The base configuration.
		 */
		const AppConfig& GetAppConfig() const;

		/*!
		 * Gets the sound controller.
		 *
		 * \return	The sound controller interface.
		 */
		ISoundController* GetSoundController() const;

		/*!
		 * Gets the ui application.
		 *
		 * \return	The ui application.
		 */
		IUiApp* GetUiApp() const;

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

		bool _showDebugInfo{false};

		AppConfig			_config;
		TickTracker			_tickTracker;

		IUiAppUPtr				_uiApp;
		IRendererUPtr			_renderer;
		IAtlasControllerUPtr	_atlasCtrl;
		IFactoryUPtr			_factory;
		IFontControllerUPtr		_fontCtrl;
		TestControllerUPtr		_testController;

		EAppState _state {EAppState::RUNNING};
	};
}

#endif // !__NADOR_APP_H__

