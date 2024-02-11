#include "Block.h"

namespace demo
{
    Block::Block(glm::ivec2 position, glm::ivec2 size, nador::video::EImageName img)
    : Entity<1>({position, size, img})
    {
    }

} // namespace demo
