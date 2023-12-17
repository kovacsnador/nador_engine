#include "Game.h"
#include "Audio.h"

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
    auto font = app->GetFontController()->GetFont(Fonts::SUPER_MARIO_BROS_3, FontSize::MEDIUM);
    auto soundCtrl = app->GetSoundController();
    Audio::Init(soundCtrl);

    MainScreen mainScreen(app->GetUiApp(), font, soundCtrl->CreateSoundSource(Sound::MARIO_THEME));
    return std::make_unique<Game>(app, mainScreen);
}
