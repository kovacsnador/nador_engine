#include "MainScreen.h"

demo::MainScreen::MainScreen(nador::IUiApp* uiApp)
: _uiApp(uiApp)
{
    _background = std::make_shared<nador::UiImage>(nador::video::EImageName::MARIO_BACKGROUND);
}

demo::MainScreen::~MainScreen()
{
    Stop();
}

void demo::MainScreen::Start()
{
    _uiApp->AddElementToLayer(nador::EUiLayer::BACKGROUND, _background.get());
}

void demo::MainScreen::Stop()
{
    _uiApp->RemoveElementFromLayer(nador::EUiLayer::BACKGROUND, _background.get());
}
