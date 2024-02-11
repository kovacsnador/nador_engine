#ifndef __NADOR_DEMO_ENTITY_H__
#define __NADOR_DEMO_ENTITY_H__

#include <array>
#include <algorithm>
#include <variant>

#include "nador/utils/Types.h"

#include "EImageName.h"

namespace demo
{
    struct Tile
    {
        glm::vec2               pos;
        nador::video::EImageName imageName;

        bool IsCollide(const glm::vec2& p, const glm::vec2& size = { 40, 40 }) const
        {
            return (p.x > pos.x && p.x < pos.x + size.x && p.y > pos.y && p.y < pos.y + size.y);
        }
    };

    template <size_t N>
    struct Entity
    {
        std::array<Tile, N> tiles;

        void Move(const glm::vec2& move)
        {
            std::for_each(tiles.begin(), tiles.end(), [&move](auto& t) { t.pos += move; });
        }

        std::tuple<bool, glm::vec2> IsCollide(const glm::vec2& from, const glm::vec2& to, glm::vec2 size = { 40, 40 }) const
        {
            glm::vec2 destination = to;

            // Calculate direction vector
            //glm::vec2 direction = from - to;
            glm::vec2 direction = to - from;

            // Calculate the distance between the two points
            float distance = glm::length(glm::vec2(direction));

            // Normalize the direction vector
            glm::vec2 normalizedDirection = glm::normalize(glm::vec2(direction));

            // Number of points to generate (pixel by pixel)
            auto numPoints = static_cast<int>(distance) + 1;

            bool collide { false };
            std::for_each(tiles.begin(), tiles.end(), [&destination, &from, /*&to,*/ &size, &collide, numPoints, normalizedDirection](const auto& t) mutable {
                // Calculate and print intermediate points
                for (int i = 0; i < numPoints && collide == false; ++i)
                {
                    // Calculate interpolated point
                    //glm::vec2 interpolatedPoint = to + glm::vec2{i * normalizedDirection.x, i * normalizedDirection.y};
                    glm::vec2 interpolatedPoint = from + glm::vec2{i * normalizedDirection.x, i * normalizedDirection.y};

                    // top left
                    collide |= t.IsCollide(interpolatedPoint, size);
                    
                    // bottom right
                    collide |= t.IsCollide(interpolatedPoint + size, size);

                    // top right
                    collide |= t.IsCollide(interpolatedPoint + glm::vec2{size.x, 0}, size);
                    
                    // bottom left
                    collide |= t.IsCollide(interpolatedPoint + glm::vec2{0, size.y}, size);

                    if(collide == false)
                    {
                        destination = interpolatedPoint;
                    }
                }
            });

            return {collide, destination};
        }
        
        template<size_t M>
        std::tuple<bool, glm::vec2> IsCollide(const Entity<M>& other, const glm::vec2& move, glm::vec2 size = { 40, 40 }) const
        {
            for(const auto& it : other.tiles)
            {
                auto [collide, pos] = IsCollide(it.pos, it.pos + move, size);

                if(collide)
                {
                    return {collide, pos - it.pos};
                }
            }
            return {false, move};
        }
    };

    using GameElement = std::variant<Entity<1>, Entity<2>, Entity<3>, Entity<4>>;

} // namespace demo

#endif //!__NADOR_DEMO_ENTITY_H__