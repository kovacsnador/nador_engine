#ifndef __NADOR_DEMO_MARIO_H__
#define __NADOR_DEMO_MARIO_H__

#include <unordered_map>

#include "Entity.h"
#include "ObjectMovement.h"

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
        Mario(glm::ivec2 position, nador::onTick_event_t& event);

        Mario(Mario&&)            = default;
        Mario& operator=(Mario&&) = default;

        void MoveForward(const std::vector<GameElement>& map);
        void MoveBackward(const std::vector<GameElement>& map);
        void Stop(const std::vector<GameElement>& map);
        void Jump(const std::vector<GameElement>& map);
        void JumpTeardown(const std::vector<GameElement>& map);

    private:
        void MoveForDirection(glm::vec2 direction, const std::vector<GameElement>& map);
        void Stop(glm::vec2 direction, const std::vector<GameElement>& map);

        std::unordered_map<glm::vec2, std::unique_ptr<ObjectMovement<Mario>>, Vec2Hash> _movements;
        std::unique_ptr<ObjectMovement<Mario>>                                          _teardownMovement;
        std::unique_ptr<ObjectMovement<Mario>>                                          _teardownJump;

        nador::onTick_event_t* _onTickEvent;

        //ObjectMovement<Mario> _gravity;
    };

    struct BigMario : Entity<2>
    {
    };
} // namespace demo

#endif //!__NADOR_DEMO_MARIO_H__