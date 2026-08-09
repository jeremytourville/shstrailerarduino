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

    const uint8_t pin_;
    Timer debounceTimer_;
    Timer longPressTimer_;
    bool longPressPending_ = false;
    int state_;
    int lastState_;
    Vector<ButtonObserver*> observers_;
};

}  // namespace shstrailer