#include "button.h"

#include "button_observer.hpp"

namespace shstrailer {

Button::Button(const uint8_t pin) : pin_(pin) {
    pinMode(pin_, INPUT_PULLUP);
    lastDebounceState_ = state_ = digitalRead(pin_);
}

void Button::update() {
    const int currentState = digitalRead(pin_);

    // reset timer when the state changes
    if (currentState != lastDebounceState_) {
        debounceTimer_.start();
        lastDebounceState_ = currentState;

        // no sense in checking timer or state change below since it just
        // happened.
        return;
    }

    // wait long enough for the state to be stable
    if (debounceTimer_.elapsed() > kDebounceDuration &&
        currentState != state_) {
        state_ = currentState;

        // the long press event was broken, it is now a pressed event
        if (HIGH == state_) {
            if (longPressPending_) {
                notifyPressed();
            }

            notifyReleased();
        }

        longPressPending_ = LOW == state_;

        if (longPressPending_) {
            longPressTimer_.start();
        }
    }

    if (longPressPending_ && longPressTimer_.elapsed() > kLongPressDuration) {
        // only notify long press once per occurrence
        longPressPending_ = false;

        notifyLongPressed();
    }

    if (LOW == state_) {
        notifyContinuousPress();
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
        observer->onPressed(pin_);
    }
}

void Button::notifyLongPressed() {
    for (auto observer : observers_) {
        observer->onLongPressed(pin_);
    }
}

void Button::notifyContinuousPress() {
    for (auto observer : observers_) {
        observer->onContinuousPress(pin_);
    }
}

void Button::notifyReleased() {
    for (auto observer : observers_) {
        observer->onReleased(pin_);
    }
}

}  // namespace shstrailer
