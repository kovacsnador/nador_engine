#include "World.h"

namespace demo
{
    void World::OnRender(nador::IRenderer* renderer)
    {
        renderer->ClearColorRGBA(_backgroundColor);

        // render map
        const auto& mapData = _map->GetMap();
        std::for_each(mapData.begin(), mapData.end(), [this, renderer](const GameElement& e) {
            std::visit([this, renderer](const auto& entity) { _drawer(renderer, entity); }, e);
        });

        // render mario
        _drawer(renderer, _mario);
    }

    void World::OnKeyPressed(nador::EKeyCode keyCode)
    {
        /*float_t acceleration = 1000.f;
        float_t maxVelocity = 400.f;
        float_t startVelocity = 0;*/

        switch (keyCode)
        {
        case nador::EKeyCode::SPACE:
            _mario.Jump();
            /*{
                Momentum momentum { { 0, -1 }, 3000, 0, 1000};
                auto movement = std::make_unique<ObjectMovement<Mario>>(_mario, momentum, LinearDecelerationWithLimit, nador::g_onTickEvent, [this]() {
                    _marioMovements.erase(nador::EKeyCode::SPACE);
                    Momentum momentum { { 0, 1 }, 3000, 3000, 0};
                    _marioTeardownMovement = std::make_unique<ObjectMovement<Mario>>(_mario, momentum, LinearAccelerationWithLimit, nador::g_onTickEvent, [this](){ _marioTeardownMovement.reset(); });
                });
                _marioMovements.insert_or_assign(nador::EKeyCode::SPACE, std::move(movement));
            }*/
            break;
        case nador::EKeyCode::LEFT:
            _mario.MoveBackward();
            /*{
                Momentum momentum { { -1., 0 }, acceleration, maxVelocity, startVelocity};
                auto movement = std::make_unique<ObjectMovement<Mario>>(_mario, momentum, LinearAccelerationWithLimit, nador::g_onTickEvent);
                _marioMovements.insert_or_assign(nador::EKeyCode::LEFT, std::move(movement));
            }*/

            break;
        case nador::EKeyCode::RIGHT:
            _mario.MoveForward();
            /*{
                Momentum momentum { { 1., 0 }, acceleration, maxVelocity, startVelocity};
                auto movement = std::make_unique<ObjectMovement<Mario>>(_mario, momentum, LinearAccelerationWithLimit, nador::g_onTickEvent);
                _marioMovements.insert_or_assign(nador::EKeyCode::RIGHT, std::move(movement));
            }*/
            break;
        default:
            break;
        }
    }

    void World::OnKeyReleased(nador::EKeyCode keyCode)
    {
        /*float_t acceleration = 2000.f;
        float_t maxVelocity = 0;
        float_t startVelocity = 400;

        auto iter = _marioMovements.find(keyCode);
        if(iter != _marioMovements.end())
        {
            startVelocity = iter->second->GetVelocity();
        }*/

        switch (keyCode)
        {
        case nador::EKeyCode::SPACE:
            _mario.JumpTeardown();
            /*{
                Momentum momentum { { 0, -1 }, 3000, 0, 1000};
                auto movement = std::make_unique<ObjectMovement<Mario>>(_mario, momentum, LinearDecelerationWithLimit, nador::g_onTickEvent, [this]() {
                    _marioMovements.erase(nador::EKeyCode::SPACE);
                    Momentum momentum { { 0, 1 }, 3000, 3000, 0};
                    _marioTeardownMovement = std::make_unique<ObjectMovement<Mario>>(_mario, momentum, LinearAccelerationWithLimit, nador::g_onTickEvent, [this](){ _marioTeardownMovement.reset(); });
                });
                _marioMovements.insert_or_assign(nador::EKeyCode::SPACE, std::move(movement));
            }*/
            break;
        case nador::EKeyCode::LEFT:
            _mario.Stop();
            /*{   
                _marioMovements.erase(keyCode);
                Momentum momentum { { -1, 0 }, acceleration, maxVelocity, startVelocity};
                _marioTeardownMovement = std::make_unique<ObjectMovement<Mario>>(_mario, momentum, LinearDecelerationWithLimit, nador::g_onTickEvent, [this](){ _marioTeardownMovement.reset(); });
            }*/
            break;
        case nador::EKeyCode::RIGHT:
            _mario.Stop();
            /*{
                _marioMovements.erase(keyCode);
                Momentum momentum { { 1., 0 }, acceleration, maxVelocity, startVelocity};
                _marioTeardownMovement = std::make_unique<ObjectMovement<Mario>>(_mario, momentum, LinearDecelerationWithLimit, nador::g_onTickEvent, [this](){ _marioTeardownMovement.reset(); });
            }*/
            break;
        default:
            break;
        }
    }

} // namespace demo
