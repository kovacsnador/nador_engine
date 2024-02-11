#ifndef __NADOR_DEMO_GRAVITY_H__
#define __NADOR_DEMO_GRAVITY_H__

#include "Entity.h"
#include "ObjectMovement.h"

namespace demo
{
    class Gravity
    {
    public:
        Gravity(Momentum momentum)
        : _momentum(momentum)
        {
        }
    
    private:
        Momentum _momentum;
    };

} // namespace demo


#endif //!__NADOR_DEMO_GRAVITY_H__