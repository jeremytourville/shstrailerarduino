#pragma once

#include <stdint.h>

namespace shstrailer {

class ButtonObserver {
   public:
    virtual ~ButtonObserver() = default;

    // This is a normal button press event. It is triggered when the button is
    // released after being pressed.
    virtual void onButtonPressed([[maybe_unused]] uint8_t pin) {};

    // This is a long button press event. It is triggered when the button has
    // been held down for a certain duration.
    virtual void onButtonLongPressed([[maybe_unused]] uint8_t pin) {};

    // This is a button down event. It is triggered when the button is pressed
    // immediately.
    virtual void onButtonDown([[maybe_unused]] uint8_t pin) {};

    // This is a button release event. It is triggered when the button is
    // released after being pressed.
    virtual void onButtonReleased([[maybe_unused]] uint8_t pin) {};
};

}  // namespace shstrailer
