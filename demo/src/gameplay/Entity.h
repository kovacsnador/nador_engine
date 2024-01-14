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
        glm::ivec2               pos;
        nador::video::EImageName imageName;

        bool IsCollide(const glm::ivec2& p, const glm::ivec2& size = { 40, 40 })
        {
            return (p.x >= pos.x && p.x <= pos.x + size.x && p.y >= pos.y && p.y <= pos.y + size.y);
        }
    };

    template <size_t N>
    struct Entity
    {
        std::array<Tile, N> tiles;

        void Move(const glm::ivec2& move)
        {
            std::for_each(tiles.begin(), tiles.end(), [&move](auto& t) { t.pos += move; });
        }

        bool IsCollide(const glm::ivec2& from, const glm::ivec2& to, glm::ivec2 size = { 40, 40 })
        {
            // Calculate direction vector
            glm::ivec2 direction = from - to;

            // Calculate the distance between the two points
            float distance = glm::length(glm::vec2(direction));

            // Normalize the direction vector
            glm::vec2 normalizedDirection = glm::normalize(glm::vec2(direction));

            // Number of points to generate (pixel by pixel)
            auto numPoints = static_cast<int>(distance) + 1;

            bool collide { false };
            std::for_each(tiles.begin(), tiles.end(), [&to, &size, &collide, numPoints, normalizedDirection](const auto& t) {
                // Calculate and print intermediate points
                for (int i = 0; i < numPoints && collide == false; ++i)
                {
                    // Calculate interpolated point
                    glm::ivec2 interpolatedPoint = to + glm::ivec2{i * normalizedDirection.x, i * normalizedDirection.y}; 
                    collide = t.IsCollide(interpolatedPoint, size);
                }
            });

            return collide;
        }
    };

    using GameElement = std::variant<Entity<1>, Entity<2>, Entity<3>, Entity<4>>;

} // namespace demo

#endif //!__NADOR_DEMO_ENTITY_H__