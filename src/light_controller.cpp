#include "light_controller.h"

#include "light.h"

namespace shstrailer {

void LightController::registerLight(Light* light) {
    if (nullptr == light) {
        Abort(F("light is nullptr"));
    }

    lights_.push_back(light);
}

void LightController::onLongPressed() { off(); }

void LightController::off() {
    for (auto light : lights_) {
        light->off();
    }
}

}  // namespace shstrailer
