#include "light.h"

namespace shstrailer {

Light::Light(const uint8_t pin) : pin_(pin) {}

void Light::onPressed() {
    initialize();

    state_ = !state_;
    write();
}

void Light::on() {
    initialize();

    state_ = HIGH;
    write();
}

void Light::off() {
    initialize();

    offInternal();
}

void Light::offInternal() {
    state_ = LOW;
    write();
}

void Light::write() { digitalWrite(pin_, state_); }

void Light::initialize() {
    if (kUninitialized == state_) {
        pinMode(pin_, OUTPUT);

        // call offInternal instead of off() to avoid calling initialize() again
        offInternal();
    }
}

}  // namespace shstrailer
