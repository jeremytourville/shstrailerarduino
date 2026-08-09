#include <Arduino.h>

#include "array.h"
#include "button.h"
#include "config.h"
#include "console.h"
#include "light.h"
#include "light_controller.h"
#include "pins.h"
#include "safety.h"
#include "types.h"
#include "version.h"
#include "winch.h"

using namespace shstrailer;

//
// Buttons
//
Button button1A(L1_SW_A);
Button button1B(L1_SW_B);
Button button2A(L2_SW_A);
Button button2B(L2_SW_B);
Button button3A(L3_SW_A);
Button button3B(L3_SW_B);
Button button4A(L4_SW_A);
Button button4B(L4_SW_B);

Button ledStripButton(LED_STRIP_SW);
Button podLightButton(POD_LIGHT_SW);

Button winchUpButton(WINCH_UP_SW);
Button winchDownButton(WINCH_DN_SW);

Array<Button*, 12> allButtons = {
    &button1A,       &button1B,       &button2A,      &button2B,
    &button3A,       &button3B,       &button4A,      &button4B,
    &ledStripButton, &podLightButton, &winchUpButton, &winchDownButton};

//
// Lights
//
Light light1(LIGHT1_OUT);
Light light2(LIGHT2_OUT);
Light light3(LIGHT3_OUT);
Light light4(LIGHT4_OUT);
Light ledStrip(LED_STRIP_OUT);
Light podLight(POD_LIGHT_OUT);

Array<Light*, 6> allLights = {&light1, &light2,   &light3,
                              &light4, &ledStrip, &podLight};

LightController lightController;
WinchController winch;
SafetyController safety;

void setup() {
    cout << endl << FW_NAME << " " << GetVersionString() << endl;

    // Initialize output-owning controllers first so outputs are immediately
    // configured and forced to their safe OFF states.
    safety.begin();
    winch.begin();

    for (auto light : allLights) {
        lightController.registerLight(light);
    }

    lightController.off();

    safety.safeStartup();

    // map buttons to lights
    button1A.registerObserver(&light1);
    button1B.registerObserver(&light1);
    button2A.registerObserver(&light2);
    button2B.registerObserver(&light2);
    button3A.registerObserver(&light3);
    button3B.registerObserver(&light3);
    button4A.registerObserver(&light4);
    button4B.registerObserver(&light4);
    ledStripButton.registerObserver(&ledStrip);
    podLightButton.registerObserver(&podLight);

    // register the light controller to turn off all lights on long press
    for (auto button : allButtons) {
        button->registerObserver(&lightController);
    }

    cout << F("System Ready") << endl;
}

void loop() {
    // Update all buttons.
    for (auto button : allButtons) {
        button->update();
    }

    // Winch is hold-to-run.
    // If both direction buttons are pressed, command STOP.
    const bool winchUpPressed = winchUpButton.isPressed();
    const bool winchDownPressed = winchDownButton.isPressed();

    if (winchUpPressed && !winchDownPressed) {
        winch.commandUp();
    } else if (winchDownPressed && !winchUpPressed) {
        winch.commandDown();
    } else {
        winch.stop();
    }

    winch.update();

    safety.setWinchFault(winch.isFaulted());
    safety.setWinchCooldown(winch.isCoolingDown());
    safety.update();
}
