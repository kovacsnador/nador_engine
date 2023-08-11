#ifndef __UTILS_TIME_H__
#define __UTILS_TIME_H__

#include <chrono>

namespace nador
{
    namespace time = std::chrono;

	using clock_t = std::chrono::high_resolution_clock;

    using nanosec_t = std::chrono::nanoseconds;
    using microsec_t = std::chrono::microseconds;
    using millisec_t = std::chrono::milliseconds;
    using seconds_t = std::chrono::seconds;
    using seconds_f_t = std::chrono::duration<float>;
    using minutes_t = std::chrono::minutes;
    using hours_t = std::chrono::hours;

    using time_point_nanosec_t = std::chrono::time_point<clock_t, nanosec_t>;
    using time_point_microsec_t = std::chrono::time_point<clock_t, microsec_t>;
    using time_point_millisec_t = std::chrono::time_point<clock_t, millisec_t>;
    using time_point_seconds_t = std::chrono::time_point<clock_t, seconds_t>;
    using time_point_seconds_f_t = std::chrono::time_point<clock_t, seconds_f_t>;
    using time_point_minutes_t = std::chrono::time_point<clock_t, minutes_t>;
    using time_point_hours_t = std::chrono::time_point<clock_t, hours_t>;
}

#endif // !__UTILS_TIME_H__

