#include "timer.h"

#include <Arduino.h>
#include <limits.h>

namespace shstrailer {

Timer::Timer() {
    static_assert(sizeof(Stamp) == 4,
                  "Timer only works with 32-bit unsigned long");

    start();
}

void Timer::start() { m_startTime = millis(); }

Timer::Stamp Timer::elapsed() const {
    const Stamp now = millis();

    // normally time will be monotonic
    if (now >= m_startTime) {
        return now - m_startTime;
    }

    // if we get here, millis() has wrapped around since start() was called
    return (ULONG_MAX - m_startTime + 1) + now;
}

}  // namespace shstrailer
