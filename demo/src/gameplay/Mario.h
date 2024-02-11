#ifndef __NADOR_DEMO_MARIO_H__
#define __NADOR_DEMO_MARIO_H__

#include <unordered_map>

#include "Entity.h"
#include "ObjectMovement.h"
#include "CollisionDetector.h"
#include "Map.h"

namespace demo
{
    struct Vec2Hash
    {
        size_t operator()(const glm::vec2& vec) const
        {
            // Combine the hashes of x and y using a simple hash function
            size_t hashX = std::hash<float> {}(vec.x);
            size_t hashY = std::hash<float> {}(vec.y);

            // A simple way to combine hashes
            return hashX ^ (hashY << 1);
        }
    };

    struct Mario : public Entity<1>
    {
        Mario(glm::ivec2 position, glm::ivec2 size, nador::onTick_event_t& event, CollisionDetector<Map> collisionDetector);

        Mario(Mario&&) = default;

        void MoveForward();
        void MoveBackward();
        void Stop();
        void Jump();
        void JumpTeardown();

    private:
        using ObjectMovement_t = ObjectMovement<Mario, CollisionDetector<Map>>;

        void MoveForDirection(glm::vec2 direction);
        void Stop(glm::vec2 direction);

        std::unordered_map<glm::vec2, std::unique_ptr<ObjectMovement_t>, Vec2Hash> _movements;
        std::unique_ptr<ObjectMovement_t>                                          _teardownMovement;
        std::unique_ptr<ObjectMovement_t>                                          _teardownJump;

        nador::onTick_event_t* _onTickEvent;

        //ObjectMovement_t _gravity;

        CollisionDetector<Map> _collisionDetector;
    };

    struct BigMario : Entity<2>
    {
    };
} // namespace demo

#endif //!__NADOR_DEMO_MARIO_H__