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

   private:
    void notifyPressed();

    void notifyLongPressed();

    void notifyDown();

    void notifyReleased();

    Vector<ButtonObserver*, 4> observers_;
    Timer debounceTimer_;
    Timer longPressTimer_;
    int state_ = LOW;
    int lastDebounceState_ = LOW;
    const uint8_t pin_;
    bool longPressPending_ = false;
};

}  // namespace shstrailer
