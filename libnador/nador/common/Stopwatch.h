#ifndef __NADOR_STOPWATCH_H__
#define __NADOR_STOPWATCH_H__

#include <chrono>

namespace nador
{
    template<typename ClockTy>
    class Stopwatch
    {
    public:
        Stopwatch()
        {
            Start();
        }

        template<typename DurationTy>
        DurationTy Stop()
        {
            return std::chrono::duration_cast<DurationTy>(ClockTy::now() - _start);
        }

        template<typename DurationTy>
        DurationTy GetPartTime()
        {
            auto now = ClockTy::now();
            auto result = std::chrono::duration_cast<DurationTy>(now - _partTime);
            _partTime = now;
            return result;
        }

        void Start()
        {
            _start = ClockTy::now();
            _partTime = _start;
        }

    private:
        std::chrono::time_point<ClockTy> _start;
        std::chrono::time_point<ClockTy> _partTime;
    };
}

#endif //!__NADOR_STOPWATCH_H__