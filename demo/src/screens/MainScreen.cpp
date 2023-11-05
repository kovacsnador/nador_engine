#include "MainScreen.h"

demo::MainScreen::MainScreen(nador::IUiApp* uiApp)
: _uiApp(uiApp)
, _background({0,0}, {0,0}, nador::video::EImageName::MARIO_BACKGROUND, {nador::EHorizontalAlignment::STRETCH, nador::EVerticalAlignment::STRETCH})
{
}

demo::MainScreen::~MainScreen()
{
    Stop();
}

void demo::MainScreen::Start()
{
    _uiApp->AddElementToLayer(nador::EUiLayer::BACKGROUND, &_background);
}

void demo::MainScreen::Stop()
{
    _uiApp->RemoveElementFromLayer(nador::EUiLayer::BACKGROUND, &_background);
}
