#include "safe_timer.h"

#include <Arduino.h>
#include <limits.h>

namespace shstrailer {

SafeTimer::SafeTimer() {
    static_assert(sizeof(unsigned long) == 4,
                  "SafeTimer only works with 32-bit unsigned long");

    start();
}

void SafeTimer::start() { m_startTime = millis(); }

unsigned long SafeTimer::elapsed() const {
    const unsigned long now = millis();

    // normally time will be monotonic
    if (now >= m_startTime) {
        return now - m_startTime;
    }

    // if we get here, millis() has wrapped around since start() was called
    return (ULONG_MAX - m_startTime + 1) + now;
}

}  // namespace shstrailer
