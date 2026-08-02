#include <Arduino.h>

#include "config.h"
#include "debounce.h"
#include "lighting.h"
#include "pins.h"
#include "safety.h"
#include "types.h"
#include "winch.h"

using namespace shstrailer;

DebouncedButton light1A(L1_SW_A);
DebouncedButton light1B(L1_SW_B);
DebouncedButton light2A(L2_SW_A);
DebouncedButton light2B(L2_SW_B);
DebouncedButton light3A(L3_SW_A);
DebouncedButton light3B(L3_SW_B);
DebouncedButton light4A(L4_SW_A);
DebouncedButton light4B(L4_SW_B);
DebouncedButton ledStripButton(LED_STRIP_SW);
DebouncedButton podLightButton(POD_LIGHT_SW);
DebouncedButton winchUpButton(WINCH_UP_SW);
DebouncedButton winchDownButton(WINCH_DN_SW);

LightingController lighting;
WinchController winch;
SafetyController safety;

void setup() {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.begin(SERIAL_BAUD_RATE);
        Serial.println();
        Serial.println(FW_NAME);
    }

    safety.begin();
    lighting.begin();
    winch.begin();

    light1A.begin();
    light1B.begin();
    light2A.begin();
    light2B.begin();
    light3A.begin();
    light3B.begin();
    light4A.begin();
    light4B.begin();
    ledStripButton.begin();
    podLightButton.begin();
    winchUpButton.begin();
    winchDownButton.begin();

    safety.safeStartup();

    if (ENABLE_SERIAL_DEBUG) {
        Serial.println(F("System Ready"));
    }
}

void loop() {
    light1A.update();
    light1B.update();
    light2A.update();
    light2B.update();
    light3A.update();
    light3B.update();
    light4A.update();
    light4B.update();
    ledStripButton.update();
    podLightButton.update();
    winchUpButton.update();
    winchDownButton.update();

    if (light1A.wasPressed() || light1B.wasPressed()) {
        lighting.toggle(LightCircuit::LIGHT1);
    }

    if (light2A.wasPressed() || light2B.wasPressed()) {
        lighting.toggle(LightCircuit::LIGHT2);
    }

    if (light3A.wasPressed() || light3B.wasPressed()) {
        lighting.toggle(LightCircuit::LIGHT3);
    }

    if (light4A.wasPressed() || light4B.wasPressed()) {
        lighting.toggle(LightCircuit::LIGHT4);
    }

    if (ledStripButton.wasPressed()) {
        lighting.toggle(LightCircuit::LED_STRIP);
    }

    if (podLightButton.wasPressed()) {
        lighting.toggle(LightCircuit::POD_LIGHT);
    }

    if (winchUpButton.isPressed()) {
        winch.commandUp();
    } else if (winchDownButton.isPressed()) {
        winch.commandDown();
    } else {
        winch.stop();
    }

    lighting.update();
    winch.update();
    safety.update();
}
