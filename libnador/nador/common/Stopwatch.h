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

        void Start()
        {
            _start = ClockTy::now();
        }

    private:
        std::chrono::time_point<ClockTy> _start;
    };
}

#endif //!__NADOR_STOPWATCH_H__