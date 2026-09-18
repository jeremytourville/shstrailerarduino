#include "light.h"

#include "halt.h"

namespace shstrailer {

Light::Light(const uint8_t pin) : pin_(pin) {
    pinMode(pin_, OUTPUT);
    off();
}

void Light::onButtonPressed([[maybe_unused]] const uint8_t pin) {
    write(HIGH == state_ ? LOW : HIGH);
}

void Light::on() { write(HIGH); }

void Light::off() { write(LOW); }

void Light::write(const uint8_t newState) {
    digitalWrite(pin_, newState);
    state_ = newState;

    notify();
}

uint8_t Light::getPin() const { return pin_; }

void Light::registerObserver(LightObserver* observer) {
    if (nullptr == observer) {
        Halt(F("light observer nullptr"));
    }

    observers_.push_back(observer);
}

void Light::notify() {
    for (auto observer : observers_) {
        observer->onLightState(pin_, state_);
    }
}

}  // namespace shstrailer
