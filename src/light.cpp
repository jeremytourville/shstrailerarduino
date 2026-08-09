#include "light.h"

namespace shstrailer {

Light::Light(const uint8_t pin) : pin_(pin) {}

void Light::onPressed() { write(HIGH == state_ ? LOW : HIGH); }

void Light::on() { write(HIGH); }

void Light::off() { write(LOW); }

void Light::write(const uint8_t newState) {
    if (kUninitialized == state_) {
        pinMode(pin_, OUTPUT);
    }

    state_ = newState;

    digitalWrite(pin_, state_);
}

}  // namespace shstrailer
