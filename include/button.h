#pragma once

#include <Arduino.h>

#include "timer.h"
#include "vector.h"

namespace shstrailer {

class ButtonObserver;

class Button {
   public:
    explicit Button(uint8_t pin);

    void update();

    void registerObserver(ButtonObserver* observer);

    [[nodiscard]] bool isPressed() const { return LOW == state_; }

   private:
    void notifyPressed();

    void notifyLongPressed();

    static constexpr Timer::Stamp kDebounceDuration = 30;
    static constexpr Timer::Stamp kLongPressDuration = 3000;

    Vector<ButtonObserver*> observers_;
    Timer debounceTimer_;
    Timer longPressTimer_;
    int state_;
    int lastState_;
    const uint8_t pin_;
    bool longPressPending_ = false;
};

}  // namespace shstrailer
