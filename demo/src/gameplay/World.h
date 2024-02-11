#ifndef __NADOR_DEMO_WORLD_H__
#define __NADOR_DEMO_WORLD_H__

#include <vector>
#include <memory>

#include "Map.h"
#include "Drawer.h"
#include "Mario.h"
#include "ObjectMovement.h"
#include "CollisionDetector.h"

#include "nador/common/KeyCodes.h"

namespace demo
{
    class World
    {
    public:
        World(std::shared_ptr<Map> map, Drawer drawer, const glm::ivec2& marioStartPos, CollisionDetector<Map> collisionDetector, glm::vec4 bgcolor = { 0.0f, 0.0f, 0.3f, 1.0f })
        : _map(std::move(map))
        , _drawer(std::move(drawer))
        , _backgroundColor(bgcolor)
        , _mario(marioStartPos, {40, 40}, nador::g_onTickEvent, collisionDetector)
        , _collisionDetector(collisionDetector)
        {
        }

        void OnRender(nador::IRenderer* renderer);
        void OnKeyPressed(nador::EKeyCode keyCode);
        void OnKeyReleased(nador::EKeyCode keyCode);

    private:
        std::shared_ptr<Map>    _map;
        //Map       _map;
        
        Drawer    _drawer;
        glm::vec4 _backgroundColor;

        Mario _mario;
        CollisionDetector<Map>  _collisionDetector;


        /*std::vector<ObjectMovement<Mario>> _movements;

        std::unordered_map<nador::EKeyCode, std::unique_ptr<ObjectMovement<Mario>>> _marioMovements;
        std::unique_ptr<ObjectMovement<Mario>> _marioTeardownMovement;*/


    };
} // namespace demo

#endif //!__NADOR_DEMO_WORLD_H__