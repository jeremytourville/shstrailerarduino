#include "light_controller.h"

#include "light.h"

namespace shstrailer {

void LightController::registerLight(const uint8_t pin) {
    lights_.emplace_back(pin);
}

void LightController::onLongPressed([[maybe_unused]] const uint8_t pin) {
    off();
}

void LightController::off() {
    for (auto& light : lights_) {
        light.off();
    }
}

Light* LightController::getLightByPin(const uint8_t pin) {
    for (auto& light : lights_) {
        if (light.getPin() == pin) {
            return &light;
        }
    }

    Abort(F("light not registered for pin"));

    return nullptr;
}

}  // namespace shstrailer
