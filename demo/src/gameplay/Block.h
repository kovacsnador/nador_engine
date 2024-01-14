#ifndef __NADOR_DEMO_BLOCK_H__
#define __NADOR_DEMO_BLOCK_H__

#include "Entity.h"

namespace demo
{
    class Block : public Entity<1>
    {
    public:
        Block(glm::ivec2 position, nador::video::EImageName img);
    };
} // namespace demo

#endif //! __NADOR_DEMO_BLOCK_H__