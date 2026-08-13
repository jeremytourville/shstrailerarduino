#pragma once

#include <stdint.h>

#include "light.h"
#include "observers/button_observer.h"
#include "vector.h"

namespace shstrailer {

class LightController : public ButtonObserver {
   public:
    void registerLight(uint8_t pin);

    void onButtonLongPressed(uint8_t pin) override;

    void off();

    [[nodiscard]] Light* getLightByPin(uint8_t pin);

   private:
    Vector<Light, 6> lights_;
};

}  // namespace shstrailer
