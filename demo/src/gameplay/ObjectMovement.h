#ifndef __NADOR_DEMO_OBJECT_MOVER_H__
#define __NADOR_DEMO_OBJECT_MOVER_H__

#include <type_traits>
#include <optional>

#include "nador/common/GlobalEvents.h"
#include "nador/common/Finally.h"

namespace demo
{
    struct Momentum
    {
        glm::vec2 direction;
        float_t   acceleration;
        float_t   maxVelocity; // pixel/sec
        float_t   startVelocity;
    };

    template <typename MovementT>
    class TearDown
    {
    public:
        TearDown(MovementT& movement, float_t time)
        : _movement(movement)
        {
            _movement._momentum.direction *= -1;
            _movement._momentum.acceleration *= -0.2;
            _movement._momentum.maxVelocity = time;
        }

    private:
        MovementT& _movement;
    };

    template <typename ObjT, typename CollisionDetectorT, typename std::enable_if<!std::is_reference_v<ObjT> && !std::is_const_v<ObjT>>* = nullptr>
    class ObjectMovement
    {
    public:
        using VelocityCb_t       = std::function<float_t(float_t /*velocity*/, float_t /*deltaTime*/, const Momentum&)>;
        using OnDoneCb_t         = std::function<void(float_t /*velocity*/)>;

        ObjectMovement(ObjT&                  obj,
                       const Momentum&        momentum,
                       VelocityCb_t           cb,
                       nador::onTick_event_t& event,
                       OnDoneCb_t             onDone    = {},
                       std::optional<CollisionDetectorT>   collisionDetector = {})
        : _obj(&obj)
        , _onTickListener(event, [this](auto d) { _OnTick(d); })
        , _momentum(momentum)
        , _velocity(momentum.startVelocity)
        , _cb(cb)
        , _onDone(onDone)
        , _collisionDetector(collisionDetector)
        {
        }

        ObjectMovement(const ObjectMovement& other)
        : _obj(other._obj)
        , _onTickListener(other._onTickListener)
        , _momentum(other._momentum)
        , _velocity(other._velocity)
        , _cb(other._cb)
        , _onDone(other._onDone)
        , _collisionDetector(other._collisionDetector)
        {
            _onTickListener.SetCallback([this](auto d) { _OnTick(d); });
        }

        ObjectMovement(ObjectMovement&& other)
        : _obj(std::move(other._obj))
        , _onTickListener(std::move(other._onTickListener))
        , _momentum(std::move(other._momentum))
        , _velocity(std::move(other._velocity))
        , _cb(std::move(other._cb))
        , _onDone(std::move(other._onDone))
        , _collisionDetector(std::move(other._collisionDetector))
        {
            _onTickListener.SetCallback([this](auto d) { _OnTick(d); });
        }

        float_t GetVelocity() const noexcept { return _velocity; }

    private:
        void _OnTick(float_t deltaTime)
        {
            _velocity     = _cb(_velocity, deltaTime, _momentum);
            auto movement = _momentum.direction * _velocity * deltaTime;

            if (_collisionDetector)
            {
                auto [isCollide, moveAllow] = _collisionDetector.value().CheckCollision(*_obj, movement);
                if (isCollide)
                {
                    auto onFinal = nador::Finally([this, velo = _velocity] {
                        if (_onDone)
                        {
                            _onDone(velo);
                        }    
                    });

                    _obj->Move(moveAllow);

                    _velocity = 0;
                    return;
                }
            }

            _obj->Move(movement);

            auto onFinal = nador::Finally([this] {
                if (_velocity == _momentum.maxVelocity && _onDone)
                {
                    _onDone(_velocity);
                }    
            });
        }

        ObjT*                    _obj;
        nador::onTick_listener_t _onTickListener;
        Momentum                 _momentum;
        float_t                  _velocity;
        VelocityCb_t             _cb;
        OnDoneCb_t               _onDone {};
        std::optional<CollisionDetectorT> _collisionDetector{nullptr};
    };

    inline float_t LinearAcceleration(float_t velocity, float_t deltaTime, const Momentum& momentum)
    {
        return velocity + momentum.acceleration * deltaTime;
    }

    inline float_t LinearDeceleration(float_t velocity, float_t deltaTime, const Momentum& momentum)
    {
        return velocity - momentum.acceleration * deltaTime;
    }

    inline float_t LinearDecelerationWithLimit(float_t velocity, float_t deltaTime, const Momentum& momentum)
    {
        return std::max(LinearDeceleration(velocity, deltaTime, momentum), momentum.maxVelocity);
    }

    inline float_t LinearAccelerationWithLimit(float_t velocity, float_t deltaTime, const Momentum& momentum)
    {
        return std::min(LinearAcceleration(velocity, deltaTime, momentum), momentum.maxVelocity);
    }

} // namespace demo

#endif //!__NADOR_DEMO_OBJECT_MOVER_H__