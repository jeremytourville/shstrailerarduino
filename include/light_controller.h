#pragma once

#include "button_observer.hpp"
#include "vector.h"

namespace shstrailer {

class Light;

class LightController : public ButtonObserver {
   public:
    void registerLight(Light* light);

    void onLongPressed(uint8_t pin) override;

    void off();

    [[nodiscard]] Light* getLightByPin(uint8_t pin);

   private:
    Vector<Light*> lights_;
};

}  // namespace shstrailer
