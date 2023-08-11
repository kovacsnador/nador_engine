#ifndef __NADOR_I_APP_H__
#define __NADOR_I_APP_H__

#include "nador/utils/Types.h"
#include "nador/common/AppConfig.h"

namespace nador
{
    class IVideo;
    class ITestController;
    class IRenderer;
    class IWindow;
    class IFileController;
    class IInputController;
    class IAtlasController;
    class IFontController;
    class ISoundController;
    class IUiApp;

    class IApp
    {
    protected:
        IApp();

    public:
        virtual ~IApp();

    public:
        static IApp* Get();

        virtual void                    InitializeDefaultTests()     = 0;
        virtual bool                    ShouldClose() const noexcept = 0;
        virtual void                    Tick()                       = 0;
        virtual void                    Run()                        = 0;
        virtual const IVideo*           GetVideo()                   = 0;
        virtual ITestController*        GetTestController()          = 0;
        virtual IRenderer*              GetRenderer()                = 0;
        virtual IWindow*                GetWindow()                  = 0;
        virtual void                    ShowDebugWindow(bool show)   = 0;
        virtual void                    ShowDebugInfo(bool show)     = 0;
        virtual bool                    IsShowDebugInfo()            = 0;
        virtual IFileController*        GetFileController() const    = 0;
        virtual const IInputController* GetInputController() const   = 0;
        virtual const IAtlasController* GetAtlasController() const   = 0;
        virtual IFontController*        GetFontController()          = 0;
        virtual const AppConfig&        GetAppConfig() const         = 0;
        virtual ISoundController*       GetSoundController() const   = 0;
        virtual IUiApp*                 GetUiApp() const             = 0;

    protected:
        static IApp* s_instance;
    };
    CREATE_PTR_TYPES(IApp);

} // namespace nador

#endif //!__NADOR_I_APP_H__
