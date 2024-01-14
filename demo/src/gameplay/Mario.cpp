#include "Mario.h"

namespace demo
{
    Mario::Mario(glm::ivec2 position)
    : Entity<1>({position, nador::video::EImageName::MARIO1})
    {
    }

} // namespace demo


