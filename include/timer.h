#pragma once

namespace shstrailer {

/*
A simple timer class that wraps the Arduino millis() function, and handles
the case where millis() wraps around after ~50 days.
*/
class Timer {
   public:
    using Stamp = unsigned long;

    Timer();

    // captures the current time as the start time for the timer.
    void start();

    // return elapsed time in milliseconds, since start() was called.
    [[nodiscard]] Stamp elapsed() const;

   private:
    Stamp startTime_;
};

}  // namespace shstrailer
