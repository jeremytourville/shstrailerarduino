#pragma once

#include <Arduino.h>

#include "pins.h"
#include "types.h"

namespace shstrailer {

class LightingController {
   public:
    LightingController();

    void begin();

    void update();

    void toggle(LightCircuit light);

    void allOff();

   private:
    Light m_lights[(uint8_t)LightCircuit::COUNT];
};

}  // namespace shstrailer
