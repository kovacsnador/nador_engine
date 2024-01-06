#ifndef __NADOR_DEMO_ENTITY_H__
#define __NADOR_DEMO_ENTITY_H__

#include <array>
#include <variant>

#include "nador/utils/Types.h"

#include "EImageName.h"

namespace demo
{
    struct Tile
    {
        glm::ivec2 pos;
        nador::video::EImageName imageName;
    };

    template<size_t N>
    struct Entity
    {
        std::array<Tile, N> tiles;
    };

    using GameElement = std::variant<Entity<1>, Entity<2>, Entity<3>, Entity<4>>; 

} // namespace demo

#endif //!__NADOR_DEMO_ENTITY_H__