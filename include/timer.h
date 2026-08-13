#pragma once

#include <limits.h>

namespace shstrailer {

/*
A simple timer class that wraps the Arduino millis() function, and handles
the case where millis() wraps around after ~50 days.
*/
class Timer {
   public:
    using Duration = unsigned long;

    Timer();

    // captures the current time as the start time for the timer.
    void start();

    // return elapsed time in milliseconds, since start() was called.
    [[nodiscard]] Duration elapsed() const;

    [[nodiscard]] inline friend Duration operator-(const Timer& lhs,
                                                   const Timer& rhs) {
        return duration(lhs.stamp_, rhs.stamp_);
    }

   private:
    using Stamp = unsigned long;

    [[nodiscard]] static Duration duration(const Stamp lhs, const Stamp rhs);

    Stamp stamp_;
};

}  // namespace shstrailer
