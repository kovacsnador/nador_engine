#include "nador/utils/TickTracker.h"

namespace nador
{
    TickTracker::TickTracker()
	: onTick_listener_t(&g_onTickEvent, std::bind(&TickTracker::_onTick, this, arg::_1))
	{
	}
    
	float_t TickTracker::GetLastDeltaTimeInSec() const
    {
        return GetLastDeltaTime();
    }

    float_t TickTracker::GetTickPerSecond() const
    {
        return GetCountPerInterval(1.f);
    }

    float_t TickTracker::GetFramePerSecond() const
    {
        return _frameTracker.GetCountPerInterval(1.f);
    }

    void TickTracker::_onTick(float_t /*deltaTime*/)  
	{
		CountUp();
	}

} // namespace nador