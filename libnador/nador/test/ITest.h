#ifndef __I_TEST_H__
#define __I_TEST_H__

#include <cmath>
#include <memory>
#include <string>

namespace nador
{
	class IRenderer;
	class IVideo;

	class ITest
	{
	protected:
		/*!
		 * Constructor.
		 */
		ITest(const IVideo* video)
		: _video(video)
		{
		}

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

	protected:
		const IVideo* _video;
	};
}

#endif // !__I_TEST_H__

