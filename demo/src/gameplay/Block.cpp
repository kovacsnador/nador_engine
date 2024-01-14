#include "Block.h"

namespace demo
{
    Block::Block(glm::ivec2 position, nador::video::EImageName img)
    : Entity<1>({position, img})
    {
    }

} // namespace demo
