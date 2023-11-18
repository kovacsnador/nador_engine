#ifndef __NADOR_DEMO_MAIN_SCREEN_H__
#define __NADOR_DEMO_MAIN_SCREEN_H__

#include "nador/ui/IUiApp.h"
#include "nador/ui/UiImage.h"

namespace demo
{
    class MainScreen
    {
    public:
        MainScreen(nador::IUiApp* uiApp);
        ~MainScreen();

        MainScreen(const MainScreen& other) = default;
        MainScreen& operator=(const MainScreen& other) = default;

        MainScreen(MainScreen&& other) = default;
        MainScreen& operator=(MainScreen&& other) = default; 

        void Start();
        void Stop();

    private:
        nador::IUiApp* _uiApp;

        std::shared_ptr<nador::UiImage> _background;
    };

} // namespace demo

#endif //!__NADOR_DEMO_MAIN_SCREEN_H__