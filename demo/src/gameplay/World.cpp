#include "World.h"

namespace demo
{
    void World::OnRender(nador::IRenderer* renderer)
    {
        renderer->ClearColorRGBA(_backgroundColor);

        const auto& mapData = _map.GetMap();
        std::for_each(mapData.begin(), mapData.end(), [this, renderer](const GameElement& e) { 
            std::visit([this, renderer](const auto& entity){ _drawer(renderer, entity); },  e);
        });
    }

} // namespace demo
