#include "World.h"

namespace demo
{
    void World::OnRender(nador::IRenderer* renderer)
    {
        renderer->ClearColorRGBA(_backgroundColor);

        // render map
        const auto& mapData = _map.GetMap();
        std::for_each(mapData.begin(), mapData.end(), [this, renderer](const GameElement& e) { 
            std::visit([this, renderer](const auto& entity){ _drawer(renderer, entity); },  e);
        });

        // render mario
        _drawer(renderer, _mario);
    }

    void World::OnKeyPressed(nador::EKeyCode keyCode)
    {
        switch (keyCode)
        {
        case nador::EKeyCode::SPACE:
            break;
        case nador::EKeyCode::LEFT:
            _mario.Move({1, 0});
            break;
        case nador::EKeyCode::RIGHT:
            _mario.Move({-1, 0});
            break;
        default:
            break;
        }
    }

} // namespace demo
