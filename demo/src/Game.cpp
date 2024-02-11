#include "Game.h"
#include "Audio.h"
#include "gameplay/Entity.h"
#include "gameplay/Drawer.h"
#include "utils/MapParser.h"

demo::Game::Game(nador::IAppPtr app, std::unique_ptr<MainScreen> mainScreen, std::unique_ptr<GameScreen> gameScreen)
: _app(app)
, _mainScreen(std::move(mainScreen))
, _gameScreen(std::move(gameScreen))
, _mainScreenEventListener(std::make_unique<nador::EventListener<size_t>>([this](size_t button) { _SwitchScreen(button); }))
{
    StartGlobalListening();
    _mainScreen->RegisterListener(*_mainScreenEventListener);
}

void demo::Game::OnAppStart()
{ 
    _mainScreen->Start();
}

void demo::Game::Suspend(bool suspend)
{
    if(suspend)
    {
        StopGlobalListening();
        _mainScreen->Stop();
    }
    else
    {
        StartGlobalListening();
        _mainScreen->Start();
    }
}

bool demo::Game::IsSuspended()
{
    return !isListening();
}

void demo::Game::_SwitchScreen(size_t button)
{
    if(button == 0)
    {
        _mainScreen->Stop();
        _gameScreen->Start();
    }
}

std::unique_ptr<demo::Game> demo::CreateGame(nador::IAppPtr app)
{
    auto font = app->GetFontController()->GetFont(Fonts::SUPER_MARIO_BROS_3, FontSize::MEDIUM);
    auto soundCtrl = app->GetSoundController();
    Audio::Init(soundCtrl);

    auto mainScreen = std::make_unique<MainScreen>(app->GetUiApp(), font, soundCtrl->CreateSoundSource(Sound::MARIO_THEME));

    auto fileCtrl = app->GetFileController();
    auto fileData = fileCtrl->Read("res/mario_maps/mario_world_1.txt");
    MapParser mapParser(fileData.value());

    auto map = std::make_shared<Map>(mapParser.GetMap());

    Drawer drawer{app->GetAtlasController()};

    CollisionDetector collisionDetector{map};

    World world{map, drawer, mapParser.GetMarioStartPosition(), collisionDetector};
    auto gameScreen = std::make_unique<GameScreen>(std::move(world));
    
    return std::make_unique<Game>(app, std::move(mainScreen), std::move(gameScreen));
}
