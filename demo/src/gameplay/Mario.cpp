#include "Mario.h"

namespace demo
{
    Mario::Mario(glm::ivec2 position, nador::onTick_event_t& event)
    : Entity<1>({ position, nador::video::EImageName::MARIO1 })
    , _onTickEvent(&event)
    //, _gravity()
    {
    }

    void Mario::MoveForward(const std::vector<GameElement>& map)
    {
        MoveForDirection({ 1, 0 }, map);
    }

    void Mario::MoveBackward(const std::vector<GameElement>& map)
    {
        MoveForDirection({ -1, 0 }, map);
    }

    void Mario::Stop(const std::vector<GameElement>& map)
    {
        if(_movements.find({-1, 0}) != _movements.end() && _movements.find({1, 0}) != _movements.end())
        {
            _movements.erase({-1, 0});
            _movements.erase({1, 0});
            return;
        }

        Stop({ -1, 0 }, map);
        Stop({ 1, 0 }, map);
    }

    void Mario::Jump(const std::vector<GameElement>& map)
    {
        auto collisionCb = GenerateCollisionCb<Entity<1>>(map);
        

        Momentum momentum { { 0, -1 }, 1, 1000.05f, 1000 };
        auto movement = std::make_unique<ObjectMovement<Mario>>(*this, momentum, LinearAccelerationWithLimit, *_onTickEvent, [this, &map](float_t velocity) {
            JumpTeardown(map);
        }, collisionCb);
        _movements.insert_or_assign({ 0, -1 }, std::move(movement));
    }

    void Mario::JumpTeardown(const std::vector<GameElement>& map)
    {
        auto collisionCb = GenerateCollisionCb<Entity<1>>(map);

        float_t acceleration = 3000;

        auto iter = _movements.find({0, -1});
        if(iter != _movements.end())
        {
            auto velocity = iter->second->GetVelocity();
            
            Momentum momentum { { 0, -1 }, acceleration, 0, velocity };
            auto movement = std::make_unique<ObjectMovement<Mario>>(*this, momentum, LinearDecelerationWithLimit, *_onTickEvent, [this, collisionCb](float_t velocity) {
                Momentum momentum { { 0, 1 }, 3000, 3000, velocity};
                _teardownJump = std::make_unique<ObjectMovement<Mario>>(*this, momentum, LinearAccelerationWithLimit, *_onTickEvent, [this](float_t) {
                    _teardownJump.reset();
                }, collisionCb);
            }, collisionCb);

            _teardownJump = std::move(movement);
            _movements.erase({ 0, -1 });
        }
    }

    void Mario::MoveForDirection(glm::vec2 direction, const std::vector<GameElement>& map)
    {
        auto collisionCb = GenerateCollisionCb<Entity<1>>(map);

        float_t acceleration  = 1000;
        float_t maxVelocity   = 400;
        float_t startVelocity = 0;

        Momentum momentum { direction, acceleration, maxVelocity, startVelocity };
        auto     movement = std::make_unique<ObjectMovement<Mario>>(*this, momentum, LinearAccelerationWithLimit, *_onTickEvent, nullptr, collisionCb);
        _movements.insert_or_assign(direction, std::move(movement));
    }

    void Mario::Stop(glm::vec2 direction, const std::vector<GameElement>& map)
    {
        auto collisionCb = GenerateCollisionCb<Entity<1>>(map);

        float_t acceleration  = 2000;
        float_t maxVelocity   = 0;
        float_t startVelocity = 400;

        auto iter = _movements.find(direction);
        if (iter != _movements.end())
        {
            startVelocity = iter->second->GetVelocity();
            _movements.erase(iter);

            Momentum momentum { direction, acceleration, maxVelocity, startVelocity };
            _teardownMovement = std::make_unique<ObjectMovement<Mario>>(
                *this, momentum, LinearDecelerationWithLimit, *_onTickEvent, [this](float_t) { _teardownMovement.reset(); }, collisionCb);
        }
    }

} // namespace demo
