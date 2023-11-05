#ifndef __NADOR_DEMO_GAME_H__
#define __NADOR_DEMO_GAME_H__

#include "nador/App.h"
#include "nador/common/GlobalListener.h"

#include "screens/MainScreen.h"

namespace demo
{
    class Game : protected nador::GlobalListener 
    {
    public:
        Game(nador::IAppPtr app);

        void OnAppStart() override;

    private:
        nador::IAppPtr _app;

        MainScreen _mainScreen;
    };
}

#endif //!__NADOR_DEMO_GAME_H__