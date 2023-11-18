#include "Game.h"

demo::Game::Game(nador::IAppPtr app, MainScreen mainScreen)
: _app(app)
, _mainScreen(std::move(mainScreen))
{
    StartGlobalListening();
}

void demo::Game::OnAppStart()
{ 
    _mainScreen.Start();
}

void demo::Game::Suspend(bool suspend)
{
    if(suspend)
    {
        StopGlobalListening();
        _mainScreen.Stop();
    }
    else
    {
        StartGlobalListening();
        _mainScreen.Start();
    }
}

bool demo::Game::IsSuspended()
{
    return !isListening();
}

std::unique_ptr<demo::Game> demo::CreateGame(nador::IAppPtr app)
{
    MainScreen mainScreen(app->GetUiApp());
    return std::make_unique<Game>(app, mainScreen);
}
