#include "GameScreen.h"

namespace demo
{
    void GameScreen::Start()
    {
        StartGlobalListening();
    }

    void GameScreen::Stop()
    {
        StopGlobalListening();
    }

    void GameScreen::OnTick(float_t) { }

    void GameScreen::OnRender(nador::IRenderer* renderer)
    {
        _world.OnRender(renderer);
    }

    void GameScreen::OnKeyPressed(nador::EKeyCode keyCode)
    {
        _world.OnKeyPressed(keyCode);
    }

    void GameScreen::OnKeyReleased(nador::EKeyCode keyCode)
    {
         _world.OnKeyReleased(keyCode);
    }

} // namespace demo
