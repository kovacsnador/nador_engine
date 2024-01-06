#ifndef __NADOR_DEMO_GAME_H__
#define __NADOR_DEMO_GAME_H__

#include "nador/App.h"
#include "nador/common/GlobalListener.h"

#include "screens/MainScreen.h"
#include "screens/GameScreen.h"

#include "Assets.h"

namespace demo
{
    class Game : protected nador::GlobalListener 
    {
    public:
        Game(nador::IAppPtr app, std::unique_ptr<MainScreen> mainScreen, std::unique_ptr<GameScreen> gameScreen);

        void OnAppStart() override;

        void Suspend(bool suspend);

        bool IsSuspended();

    private:
        void _SwitchScreen(size_t button);

        nador::IAppPtr _app;

        std::unique_ptr<MainScreen> _mainScreen;
        std::unique_ptr<GameScreen> _gameScreen;

        std::unique_ptr<nador::EventListener<size_t>> _mainScreenEventListener;
    };

    std::unique_ptr<Game> CreateGame(nador::IAppPtr app);
}

#endif //!__NADOR_DEMO_GAME_H__