#ifndef __NADOR_DEMO_GAME_H__
#define __NADOR_DEMO_GAME_H__

#include "nador/App.h"
#include "nador/common/GlobalListener.h"

#include "screens/MainScreen.h"
#include "Assets.h"

namespace demo
{
    class Game : protected nador::GlobalListener 
    {
    public:
        Game(nador::IAppPtr app, MainScreen mainScreen);

        void OnAppStart() override;

        void Suspend(bool suspend);

        bool IsSuspended();

    private:
        nador::IAppPtr _app;

        MainScreen _mainScreen;
    };

    std::unique_ptr<Game> CreateGame(nador::IAppPtr app);
}

#endif //!__NADOR_DEMO_GAME_H__