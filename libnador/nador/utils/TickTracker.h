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
		: onRender_listener_t(g_onRenderEvent, std::bind(&FrameTracker::RenderCountUp, this, arg::_1))
		{
		}

		void RenderCountUp(IRenderer*)
		{
			CountUp();
		}
	};

	class TickTracker : public onTickBegin_listener_t, public IntervalCounter<>
	{
	public:
		TickTracker();

		/*!
		 * Gets the last delta time in seconds.
		 *
		 * \return	The last delta time.
		 */
		float_t GetLastDeltaTimeInSec() const;

		template<typename DurationTy>
		auto DeltaTime() const noexcept
		{
			std::chrono::duration<float_t> deltaInSec(GetLastDeltaTime());
			return std::chrono::duration_cast<DurationTy>(deltaInSec);
		}

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
		void _onTickBegin();

		FrameTracker		_frameTracker;
	};
}

#endif // !__TICK_TRACKER_H__