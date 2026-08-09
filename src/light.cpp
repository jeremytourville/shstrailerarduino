#include "light.h"

namespace shstrailer {

Light::Light(uint8_t pin) : pin_(pin), state_(LOW) { pinMode(pin_, OUTPUT); }

void Light::onPressed() {
    state_ = !state_;
    write();
}

void Light::on() {
    state_ = HIGH;
    write();
}

void Light::off() {
    state_ = LOW;
    write();
}

void Light::write() { digitalWrite(pin_, state_); }

}  // namespace shstrailer
