#ifndef __NADOR_DEMO_MARIO_H__
#define __NADOR_DEMO_MARIO_H__

#include "Entity.h"

namespace demo
{
    struct Mario : Entity<1>
    {
        Mario(glm::ivec2 position);
    };

    struct BigMario : Entity<2>
    {
    };
} // namespace demo

#endif //!__NADOR_DEMO_MARIO_H__