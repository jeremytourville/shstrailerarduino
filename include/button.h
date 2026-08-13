#pragma once

#include <stdint.h>

#include "timer.h"
#include "vector.h"

namespace shstrailer {

class ButtonObserver;

class Button {
   public:
    explicit Button(uint8_t pin);

    void update();

    void registerObserver(ButtonObserver* observer);

   private:
    void notifyPressed();

    void notifyLongPressed();

    void notifyContinuousPress();

    void notifyReleased();

    static constexpr Timer::Stamp kDebounceDuration = 30;
    static constexpr Timer::Stamp kLongPressDuration = 2000;

    Vector<ButtonObserver*, 4> observers_;
    Timer debounceTimer_;
    Timer longPressTimer_;
    int state_ = LOW;
    int lastDebounceState_ = LOW;
    const uint8_t pin_;
    bool longPressPending_ = false;
};

}  // namespace shstrailer
