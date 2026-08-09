#include "light.h"

namespace shstrailer {

Light::Light(const uint8_t pin) : pin_(pin) {
    pinMode(pin_, OUTPUT);
    off();
}

void Light::onPressed([[maybe_unused]] const uint8_t pin) {
    write(HIGH == state_ ? LOW : HIGH);
}

void Light::on() { write(HIGH); }

void Light::off() { write(LOW); }

void Light::write(const uint8_t newState) {
    digitalWrite(pin_, newState);
    state_ = newState;
}

uint8_t Light::getPin() const { return pin_; }

}  // namespace shstrailer
