#ifndef __NADOR_DEMO_ENTITY_H__
#define __NADOR_DEMO_ENTITY_H__

#include <array>
#include <algorithm>
#include <variant>
#include <tuple>

#include "nador/utils/Types.h"

#include "EImageName.h"

namespace demo
{
    struct Tile
    {
        glm::vec2                   pos;
        glm::vec2                   size{40, 40};
        nador::video::EImageName    imageName;

        bool IsOverlapping(const Tile& other) const noexcept
        {
            // Check for overlap in each dimension (x and y)
            bool overlapX = pos.x + size.x - 1 > other.pos.x && pos.x < other.pos.x + other.size.x - 1;
            bool overlapY = pos.y + size.y - 1 > other.pos.y && pos.y < other.pos.y + other.size.y - 1;

            // If there's overlap in both dimensions, the tiles are overlapping
            return overlapX && overlapY;
        }

        bool operator==(const Tile& other) const noexcept
        {
            return std::tie(pos, size, imageName) == std::tie(other.pos, other.size, other.imageName);
        }
    };

    template <size_t N>
    struct Entity
    {
        static_assert(N > 0);

        std::array<Tile, N> tiles;

        glm::vec2 GetPosition()
        {
            // gets the first tile position
            return tiles.begin()->pos;
        }

        template<size_t OtherN>
        bool operator==(const Entity<OtherN>& other)
        {
            if constexpr (OtherN == N)
            {
                return other.tiles == tiles;
            }
            return false;
        }

        void Move(const glm::vec2& move)
        {
            std::for_each(tiles.begin(), tiles.end(), [&move](auto& t) { t.pos += move; });
        }

        void SetPosition(const glm::vec2& newPos)
        {
            auto movement = newPos - GetPosition();
            Move(movement);
        }

        template<size_t M>
        bool CheckCollision(const Entity<M>& other) const noexcept
        {
            for(const auto& tile1 : other.tiles)
            {
                for(const auto& tile2 : tiles)
                {
                    if(tile1.IsOverlapping(tile2))
                    {
                        return true;
                    }
                }
            }
            return false;
        }
    };

    using GameElement = std::variant<Entity<1>, Entity<2>, Entity<3>, Entity<4>>;

} // namespace demo

#endif //!__NADOR_DEMO_ENTITY_H__