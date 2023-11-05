#include "Game.h"

demo::Game::Game(nador::IAppPtr app)
: _app(app)
, _mainScreen(app->GetUiApp())
{
    StartGlobalListening();
}

void demo::Game::OnAppStart()
{ 
    //_mainScreen.Start();
}
