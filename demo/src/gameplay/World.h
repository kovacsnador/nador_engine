#ifndef __NADOR_DEMO_WORLD_H__
#define __NADOR_DEMO_WORLD_H__

#include <vector>

#include "Map.h"
#include "Drawer.h"
#include "Mario.h"
#include "ObjectMovement.h"

#include "nador/common/KeyCodes.h"

namespace demo
{
    class World
    {
    public:
        World(Map map, Drawer drawer, const glm::ivec2& marioStartPos, glm::vec4 bgcolor = { 0.0f, 0.0f, 0.3f, 1.0f })
        : _map(map)
        , _drawer(std::move(drawer))
        , _backgroundColor(bgcolor)
        , _mario(marioStartPos, nador::g_onTickEvent)
        {
        }

        void OnRender(nador::IRenderer* renderer);
        void OnKeyPressed(nador::EKeyCode keyCode);
        void OnKeyReleased(nador::EKeyCode keyCode);

    private:
        Map       _map;
        Drawer    _drawer;
        glm::vec4 _backgroundColor;

        Mario _mario;

        /*std::vector<ObjectMovement<Mario>> _movements;

        std::unordered_map<nador::EKeyCode, std::unique_ptr<ObjectMovement<Mario>>> _marioMovements;
        std::unique_ptr<ObjectMovement<Mario>> _marioTeardownMovement;*/


    };
} // namespace demo

#endif //!__NADOR_DEMO_WORLD_H__