#ifndef __NADOR_DEMO_WORLD_H__
#define __NADOR_DEMO_WORLD_H__

#include "Map.h"
#include "Drawer.h"

namespace demo
{
    class World
    {
    public:
        World(Map map, Drawer drawer, glm::vec4 bgcolor = {0.0f, 0.0f, 0.3f, 1.0f})
        : _map(std::move(map))
        , _drawer(std::move(drawer))
        , _backgroundColor(bgcolor)
        {
        }

        void OnRender(nador::IRenderer* renderer);
        
    private:
        Map _map;
        Drawer _drawer;

        glm::vec4 _backgroundColor;
    };
} // namespace demo

#endif //!__NADOR_DEMO_WORLD_H__