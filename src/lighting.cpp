#include "lighting.h"

#include "config.h"

namespace shstrailer {

LightingController::LightingController() {
    m_lights[(uint8_t)LightCircuit::LIGHT1] = {LIGHT1_OUT, false};
    m_lights[(uint8_t)LightCircuit::LIGHT2] = {LIGHT2_OUT, false};
    m_lights[(uint8_t)LightCircuit::LIGHT3] = {LIGHT3_OUT, false};
    m_lights[(uint8_t)LightCircuit::LIGHT4] = {LIGHT4_OUT, false};
    m_lights[(uint8_t)LightCircuit::LED_STRIP] = {LED_STRIP_OUT, false};
    m_lights[(uint8_t)LightCircuit::POD_LIGHT] = {POD_LIGHT_OUT, false};
}

void LightingController::begin() {
    for (uint8_t i = 0; i < (uint8_t)LightCircuit::COUNT; ++i) {
        pinMode(m_lights[i].outputPin, OUTPUT);
        digitalWrite(m_lights[i].outputPin, OUTPUT_OFF);
        m_lights[i].state = false;
    }
}

void LightingController::toggle(LightCircuit light) {
    const uint8_t index = (uint8_t)light;

    if (index >= (uint8_t)LightCircuit::COUNT) {
        return;
    }

    m_lights[index].state = !m_lights[index].state;
}

void LightingController::update() {
    for (uint8_t i = 0; i < (uint8_t)LightCircuit::COUNT; ++i) {
        digitalWrite(m_lights[i].outputPin,
                     m_lights[i].state ? OUTPUT_ON : OUTPUT_OFF);
    }
}

void LightingController::allOff() {
    for (uint8_t i = 0; i < (uint8_t)LightCircuit::COUNT; ++i) {
        m_lights[i].state = false;
    }

    update();
}

bool LightingController::isOn(LightCircuit light) const {
    const uint8_t index = (uint8_t)light;

    if (index >= (uint8_t)LightCircuit::COUNT) {
        return false;
    }

    return m_lights[index].state;
}

}  // namespace shstrailer
