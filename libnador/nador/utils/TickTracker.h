#ifndef __TICK_TRACKER_H__
#define __TICK_TRACKER_H__

#include "nador/common/GlobalEvents.h"
#include "nador/utils/Time.h"
#include "nador/utils/IntervalCounter.h"

namespace nador
{
	struct FrameTracker : public onRender_listener_t, public IntervalCounter<>
	{
		/*!
		 * FrameTracker constructor.
		 */
		FrameTracker()
		: onRender_listener_t(&g_onRenderEvent, std::bind(&FrameTracker::CountUp, this))
		{
		}
	};

	class TickTracker : public onTick_listener_t, public IntervalCounter<>
	{
	public:
		TickTracker();

		/*!
		 * Gets the last delta time in seconds.
		 *
		 * \return	The last delta time.
		 */
		float_t GetLastDeltaTimeInSec() const;

		/*!
		 * Gets the tick per seconds (TPS).
		 *
		 * \return	(TPS).
		 */
		float_t GetTickPerSecond() const;

		/*!
		 * Gets the frame per seconds (FPS).
		 *
		 * \return	(FPS).
		 */
		float_t GetFramePerSecond() const;

	private:
		void _onTick(float_t /*deltaTime*/);

		FrameTracker		_frameTracker;
	};
}

#endif // !__TICK_TRACKER_H__