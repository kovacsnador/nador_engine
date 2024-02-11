#include "Mario.h"

namespace demo
{
    static Momentum s_gravityMomentum{ { 0, 1 }, 3000, 3000, 0};

    Mario::Mario(glm::ivec2 position, glm::ivec2 size, nador::onTick_event_t& event, CollisionDetector<Map> collisionDetector)
    : Entity<1>({ position, size, nador::video::EImageName::MARIO1 })
    , _onTickEvent(&event)
    //, _gravity(*this, s_gravityMomentum, LinearAccelerationWithLimit, *_onTickEvent, nullptr, nullptr)
    , _collisionDetector(collisionDetector)
    {
    }

    void Mario::MoveForward()
    {
        MoveForDirection({ 1, 0 });
    }

    void Mario::MoveBackward()
    {
        MoveForDirection({ -1, 0 });
    }

    void Mario::Stop()
    {
        if(_movements.find({-1, 0}) != _movements.end() && _movements.find({1, 0}) != _movements.end())
        {
            _movements.erase({-1, 0});
            _movements.erase({1, 0});
            return;
        }

        Stop({ -1, 0 });
        Stop({ 1, 0 });
    }

    void Mario::Jump()
    {
        glm::vec2 direction{0, -1}; 
        if(_movements.contains(direction) == false && _teardownJump == nullptr)
        {
            Momentum momentum { direction, 1, 1000.05f, 1000 };
            auto movement = std::make_unique<ObjectMovement_t>(*this, momentum, LinearAccelerationWithLimit, *_onTickEvent, [this](float_t) {
                JumpTeardown();
            }, _collisionDetector );
            _movements.insert_or_assign(direction, std::move(movement));
        }
    }

    void Mario::JumpTeardown()
    {
        float_t acceleration = 3000;

        auto iter = _movements.find({0, -1});
        if(iter != _movements.end())
        {
            auto velocity = iter->second->GetVelocity();
            
            Momentum momentum { { 0, -1 }, acceleration, 0, velocity };
            auto movement = std::make_unique<ObjectMovement_t>(*this, momentum, LinearDecelerationWithLimit, *_onTickEvent, [this](float_t velocity) {
                Momentum momentum { { 0, 1 }, 3000, 300000, velocity};
                _teardownJump = std::make_unique<ObjectMovement_t>(*this, momentum, LinearAccelerationWithLimit, *_onTickEvent, [this](float_t) {
                    _teardownJump.reset();
                }, _collisionDetector);
            }, _collisionDetector);

            _teardownJump = std::move(movement);
            _movements.erase({ 0, -1 });
        }
    }

    void Mario::MoveForDirection(glm::vec2 direction)
    {
        float_t acceleration  = 1000;
        float_t maxVelocity   = 400;
        float_t startVelocity = 0;

        Momentum momentum { direction, acceleration, maxVelocity, startVelocity };
        auto     movement = std::make_unique<ObjectMovement_t>(*this, momentum, LinearAccelerationWithLimit, *_onTickEvent, nullptr, _collisionDetector);
        _movements.insert_or_assign(direction, std::move(movement));
    }

    void Mario::Stop(glm::vec2 direction)
    {
        float_t acceleration  = 2000;
        float_t maxVelocity   = 0;
        float_t startVelocity = 400;

        auto iter = _movements.find(direction);
        if (iter != _movements.end())
        {
            startVelocity = iter->second->GetVelocity();
            _movements.erase(iter);

            Momentum momentum { direction, acceleration, maxVelocity, startVelocity };
            _teardownMovement = std::make_unique<ObjectMovement_t>(
                *this, momentum, LinearDecelerationWithLimit, *_onTickEvent, [this](float_t) { _teardownMovement.reset(); }, _collisionDetector);
        }
    }

} // namespace demo
