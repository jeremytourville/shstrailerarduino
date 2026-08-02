#pragma once

namespace shstrailer {

/*
A simple timer class that wraps the Arduino millis() function, and handles
the case where millis() wraps around after ~50 days.
*/
class SafeTimer {
   public:
    SafeTimer();

    // captures the current time as the start time for the timer.
    void start();

    // return elapsed time in milliseconds, since start() was called.
    [[nodiscard]] unsigned long elapsed() const;

   private:
    unsigned long m_startTime;
};

}  // namespace shstrailer
