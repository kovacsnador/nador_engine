#include "MainScreen.h"
#include "Assets.h"
#include "Audio.h"

demo::MainScreen::MainScreen(nador::IUiApp* uiApp, std::shared_ptr<nador::Font> font, std::shared_ptr<nador::ISoundSource> mainMusic)
: _uiApp(uiApp)
, _music(std::move(mainMusic))
{
    _scene = std::make_shared<nador::UiScene>();

    _background = std::make_shared<nador::UiImage>(nador::video::EImageName::MARIO_BACKGROUND, nador::UiAlignment::Stretch(), _scene.get());

    nador::UiAlignment alignment = { nador::EHorizontalAlignment::CENTER, nador::EVerticalAlignment::TOP };

    _superMarioLabel = std::make_shared<nador::UiTextLabel>(glm::ivec2 { 0, 50 }, font, "super mario", alignment);

    _superMarioLabel->SetColor({ 1, 0, 0, 1 });
    _superMarioLabel->SetFontSize(40);

    _menuButtons[0]
        = std::make_shared<MenuButton>(glm::ivec2 { 0, 300 }, glm::ivec2 { 300, 100 }, alignment, font, "start", nador::video::EImageName::MARIO1);
    _menuButtons[1]
        = std::make_shared<MenuButton>(glm::ivec2 { 0, 400 }, glm::ivec2 { 300, 100 }, alignment, font, "options", nador::video::EImageName::MARIO1);
    _menuButtons[2]
        = std::make_shared<MenuButton>(glm::ivec2 { 0, 500 }, glm::ivec2 { 300, 100 }, alignment, font, "exit", nador::video::EImageName::MARIO1);

    _scene->AddChild(_superMarioLabel);

    std::for_each(_menuButtons.begin(), _menuButtons.end(), [this](const auto& it) { _scene->AddChild(it); });

    _menuButtons[selected]->Select(true);
}

demo::MainScreen::~MainScreen()
{
    Stop();
}

void demo::MainScreen::Start()
{
    _uiApp->AddElementToLayer(nador::EUiLayer::BACKGROUND, _scene.get());

    _keyReleasedListener.SetCallback(std::bind(&MainScreen::_OnKeyPressed, this, std::placeholders::_1));
    nador::g_onKeyReleasedEvent += _keyReleasedListener;

    if (_music)
    {
        _music->Play();
    }
}

void demo::MainScreen::Stop()
{
    _uiApp->RemoveElementFromLayer(nador::EUiLayer::BACKGROUND, _scene.get());

    nador::g_onKeyReleasedEvent -= _keyReleasedListener;

    if (_music)
    {
        _music->Stop();
    }
}

glm::ivec2 demo::MainScreen::_CalculateMarioPos(const std::shared_ptr<nador::UiTextLabel>& label) const noexcept
{
    const auto& labelPos = label->GetPosition();
    return { labelPos.x - 80, labelPos.y };
}

void demo::MainScreen::_OnKeyPressed(nador::EKeyCode keycode)
{
    switch (keycode)
    {
    case nador::EKeyCode::DOWN:
        if (selected < _menuButtons.size() - 1)
        {
            _menuButtons[selected]->Select(false);
            _menuButtons[++selected]->Select(true);
            Audio::Play(Sound::SMB_KICK);
        }
        break;
    case nador::EKeyCode::UP:
        if (selected > 0)
        {
            _menuButtons[selected]->Select(false);
            _menuButtons[--selected]->Select(true);
            Audio::Play(Sound::SMB_KICK);
        }
        break;
    default:
        break;
    }
}
