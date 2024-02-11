#ifndef __NADOR_DEMO_COLLISION_DETECTOR_H__
#define __NADOR_DEMO_COLLISION_DETECTOR_H__

#include <memory>
#include <tuple>

#include "nador/utils/Types.h"

namespace demo
{
    template <typename MapT>
    class CollisionDetector
    {
    public:
        CollisionDetector(std::shared_ptr<MapT> map)
        : _map(std::move(map))
        {
        }

        template <size_t N>
        std::tuple<bool, glm::ivec2> CheckCollision(Entity<N> obj, const glm::vec2& movement)
        {
            const glm::vec2 from =  obj.GetPosition();

            // the desired destination position
            glm::vec2 destination = from + movement;

            // Calculate direction vector
            glm::vec2 direction = destination - from;

            // Calculate the distance between the two points
            float distance = glm::length(direction);

            // Normalize the direction vector
            glm::vec2 normalizedDirection = glm::normalize(direction);

            // Number of points to generate (pixel by pixel)
            auto numPoints = static_cast<int>(distance) + 1;

            glm::vec2 lastAllowedPos = from;
            bool collide { false };
            for (int i = 0; i < numPoints && collide == false; ++i)
            {
                // moving to destination
                obj.Move(normalizedDirection);

                for (const auto& it : _map->GetMap())
                {
                    std::visit([&obj, &collide](const auto& entity) mutable { collide = entity.CheckCollision(obj); }, it);

                    if(collide)
                    {
                        goto exit;
                    }
                }
                lastAllowedPos = obj.GetPosition();
            }

        exit:
            return {collide, lastAllowedPos - from};
        }

    private:
        std::shared_ptr<MapT> _map;
    };

} // namespace demo

#endif //!__NADOR_DEMO_COLLISION_DETECTOR_H__