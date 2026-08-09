#include "light_controller.h"

#include "light.h"

namespace shstrailer {

void LightController::registerLight(Light* light) {
    if (nullptr == light) {
        Abort(F("light is nullptr"));
    }

    lights_.push_back(light);
}

void LightController::onLongPressed([[maybe_unused]] const uint8_t pin) {
    off();
}

void LightController::off() {
    for (auto light : lights_) {
        light->off();
    }
}

Light* LightController::getLightByPin(const uint8_t pin) {
    for (auto light : lights_) {
        if (light->getPin() == pin) {
            return light;
        }
    }

    Abort(F("light not registered for pin"));

    return nullptr;
}

}  // namespace shstrailer
