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

} // namespace demo
