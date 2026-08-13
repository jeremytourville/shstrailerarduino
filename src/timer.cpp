#include "timer.h"

#include <Arduino.h>

namespace shstrailer {

Timer::Timer() {
    static_assert(sizeof(Stamp) == 4,
                  "Timer only works with 32-bit unsigned long");

    start();
}

void Timer::start() { stamp_ = millis(); }

Timer::Duration Timer::elapsed() const { return duration(millis(), stamp_); }

Timer::Duration Timer::duration(const Stamp lhs, const Stamp rhs) {
    // normally time will be monotonic
    if (lhs >= rhs) {
        return lhs - rhs;
    }

    // if we get here, millis() has wrapped around since start() was called
    return (ULONG_MAX - rhs + 1) + lhs;
}

}  // namespace shstrailer
