#ifndef __UTILS_INTERVAL_COUNTER_H__
#define __UTILS_INTERVAL_COUNTER_H__

#include <array>

#include "nador/log/ILog.h"
#include "nador/utils/Types.h"
#include "nador/utils/Time.h"

namespace nador
{
	template<size_t Size = 60>
	class IntervalCounter
	{
		using delta_time_list_t = std::array<float_t, Size>;

	public:
		/*!
		 * IntervalCounter destructor.
		 */
		virtual ~IntervalCounter() = default;

		/*!
		 * Counts up and saves the delay time.
		 */
		void CountUp()
		{
			time_point_nanosec_t now = clock_t::now();

			if (_lastCountUp.time_since_epoch().count() != 0)
			{
				if (_index >= Size)
				{
					_index = 0;
				}

				seconds_f_t diff = now - _lastCountUp;
				_lastDeltaTime = diff.count();

				_deltaTimes.at(_index++) = _lastDeltaTime;
			}

			_lastCountUp = now;
		}

		/*!
		 * Gets the count in interval.
		 * 
		 * \param interval	The time interval in sec.
		 * 
		 * \return	The average count.
		 */
		float_t GetCountPerInterval(float_t interval = 1.f) const noexcept
		{
			float_t deltaTimeAverage = 0;
			size_t deltaTimeCount = 0;

			for (auto deltaTime : _deltaTimes)
			{
				if (deltaTime)
				{
					deltaTimeCount++;
					deltaTimeAverage += deltaTime;
				}
			}

			if (deltaTimeCount == 0)
			{
				return 0;
			}

			return interval / (deltaTimeAverage / deltaTimeCount);
		}

		/*!
		 * Gets the last count up timestamp.
		 * 
		 * \return	The timestamp.
		 */
		const time_point_nanosec_t& GetLastCountUpTimestamp() const noexcept
		{
			return _lastCountUp;
		}

		/*!
		 * Gets the last delta time.
		 * 
		 * \return	The last delta time.
		 */
		float_t GetLastDeltaTime() const noexcept
		{
			return _lastDeltaTime;
		}

	private:
		size_t					_index{0};
		float_t					_lastDeltaTime{0};
		delta_time_list_t		_deltaTimes{};
		time_point_nanosec_t	_lastCountUp;
	};
}

#endif // !__UTILS_INTERVAL_COUNTER_H__