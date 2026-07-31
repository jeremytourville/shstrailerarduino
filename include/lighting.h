#ifndef LIGHTING_H
#define LIGHTING_H
#include <Arduino.h>
#include "pins.h"
#include "types.h"

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

#endif
