#ifndef __I_TEST_H__
#define __I_TEST_H__

#include <cmath>
#include <memory>
#include <string>

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
		virtual void OnDebugRender() {};
	};
}

#endif // !__I_TEST_H__

