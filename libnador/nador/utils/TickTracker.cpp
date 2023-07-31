#include "nador/utils/TickTracker.h"

namespace nador
{
    TickTracker::TickTracker()
	: onTickBegin_listener_t(g_onTickBeginEvent, std::bind(&TickTracker::_onTickBegin, this))
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

    void TickTracker::_onTickBegin()  
	{
		CountUp();
	}

} // namespace nador