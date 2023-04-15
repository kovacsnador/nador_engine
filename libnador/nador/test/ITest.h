#ifndef __NADOR_I_TEST_H__
#define __NADOR_I_TEST_H__

#include <cmath>
#include <memory>
#include <string>

#include "nador/utils/Types.h"

namespace nador
{
    class IRenderer;

    class ITest
    {
    protected:
        /*!
         * Constructor.
         */
        ITest() = default;

    public:
        /*!
         * Virtual destructor.
         */
        virtual ~ITest() = default;

        /*!
         * On tick function.
         *
         * \param  The delta time between ticks.
         */
        virtual void OnTick(float_t) {};

        /*!
         * Called on render.
         */
        virtual void OnRender(IRenderer*) {};

        /*!
         * Render the imgui content.
         */
        virtual void OnDebugRender(IRenderer*) {};
    };
    CREATE_PTR_TYPES(ITest);
	
} // namespace nador

#endif // !__NADOR_I_TEST_H__
