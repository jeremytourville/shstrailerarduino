#include "button.h"

#include "button_observer.hpp"

namespace shstrailer {

Button::Button(const uint8_t pin) : pin_(pin) {}

void Button::update() {
    if (!initialized_) {
        initialized_ = true;
        pinMode(pin_, INPUT_PULLUP);
        lastState_ = state_ = digitalRead(pin_);
    }

    const int currentState = digitalRead(pin_);

    // reset timer when the state changes
    if (currentState != lastState_) {
        debounceTimer_.start();
        lastState_ = currentState;

        return;
    }

    // wait long enough for the state to be stable
    if (debounceTimer_.elapsed() > kDebounceDuration &&
        currentState != state_) {
        state_ = currentState;

        // the long press event was broken, it is now a pressed event
        if (HIGH == state_ && longPressPending_) {
            notifyPressed();
        }

        longPressPending_ = isPressed();

        if (longPressPending_) {
            longPressTimer_.start();
        }
    }

    if (longPressPending_ && longPressTimer_.elapsed() > kLongPressDuration) {
        // only notify long press once per occurrence
        longPressPending_ = false;

        notifyLongPressed();
    }
}

void Button::registerObserver(ButtonObserver* observer) {
    if (nullptr == observer) {
        Abort(F("observer is nullptr"));
    }

    observers_.push_back(observer);
}

void Button::notifyPressed() {
    for (auto observer : observers_) {
        observer->onPressed();
    }
}

void Button::notifyLongPressed() {
    for (auto observer : observers_) {
        observer->onLongPressed();
    }
}

}  // namespace shstrailer
