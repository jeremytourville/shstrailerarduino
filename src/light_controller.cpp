#include "light_controller.h"

#include "light.h"

namespace shstrailer {

void LightController::registerLight(Light* light) { lights_.push_back(light); }

void LightController::onLongPressed() { off(); }

void LightController::off() {
    for (auto light : lights_) {
        light->off();
    }
}

}  // namespace shstrailer
