#include <Arduino.h>

#include <new>

#include "button.h"
#include "config.h"
#include "console.h"
#include "light.h"
#include "light_controller.h"
#include "object_allocator.h"
#include "pins.h"
#include "safety.h"
#include "types.h"
#include "vector.h"
#include "version.h"
#include "winch.h"

using namespace shstrailer;

//
// Buttons
//
ObjectAllocator<Button, 12> buttonAllocator;

Button* winchUpButton = nullptr;
Button* winchDownButton = nullptr;
Vector<Button*> allButtons;

//
// Lights
//
ObjectAllocator<Light, 6> lightAllocator;

Light* light1 = nullptr;
Light* light2 = nullptr;
Light* light3 = nullptr;
Light* light4 = nullptr;
Light* ledStrip = nullptr;
Light* podLight = nullptr;

//
// Controllers
//
LightController lightController;
WinchController winch;
SafetyController safety;

void initializeLights(LightController& lightControllerLocal) {
    light1 = lightAllocator.allocate(LIGHT1_OUT);
    lightControllerLocal.registerLight(light1);

    light2 = lightAllocator.allocate(LIGHT2_OUT);
    lightControllerLocal.registerLight(light2);

    light3 = lightAllocator.allocate(LIGHT3_OUT);
    lightControllerLocal.registerLight(light3);

    light4 = lightAllocator.allocate(LIGHT4_OUT);
    lightControllerLocal.registerLight(light4);

    ledStrip = lightAllocator.allocate(LED_STRIP_OUT);
    lightControllerLocal.registerLight(ledStrip);

    podLight = lightAllocator.allocate(POD_LIGHT_OUT);
    lightControllerLocal.registerLight(podLight);

    lightControllerLocal.off();
}

void initializeButtons(LightController& lightControllerLocal) {
    winchUpButton = buttonAllocator.allocate(WINCH_UP_SW);
    allButtons.push_back(winchUpButton);

    winchDownButton = buttonAllocator.allocate(WINCH_DN_SW);
    allButtons.push_back(winchDownButton);

    // map buttons to lights
    allButtons.push_back(buttonAllocator.allocate(L1_SW_A));
    allButtons.back()->registerObserver(light1);
    allButtons.push_back(buttonAllocator.allocate(L1_SW_B));
    allButtons.back()->registerObserver(light1);

    allButtons.push_back(buttonAllocator.allocate(L2_SW_A));
    allButtons.back()->registerObserver(light2);
    allButtons.push_back(buttonAllocator.allocate(L2_SW_B));
    allButtons.back()->registerObserver(light2);

    allButtons.push_back(buttonAllocator.allocate(L3_SW_A));
    allButtons.back()->registerObserver(light3);
    allButtons.push_back(buttonAllocator.allocate(L3_SW_B));
    allButtons.back()->registerObserver(light3);

    allButtons.push_back(buttonAllocator.allocate(L4_SW_A));
    allButtons.back()->registerObserver(light4);
    allButtons.push_back(buttonAllocator.allocate(L4_SW_B));
    allButtons.back()->registerObserver(light4);

    allButtons.push_back(buttonAllocator.allocate(LED_STRIP_SW));
    allButtons.back()->registerObserver(ledStrip);
    allButtons.push_back(buttonAllocator.allocate(POD_LIGHT_SW));
    allButtons.back()->registerObserver(podLight);

    // register the light controller to turn off all lights on long press
    for (auto button : allButtons) {
        button->registerObserver(&lightControllerLocal);
    }
}

void setup() {
    cout << endl << FW_NAME << " " << GetVersionString() << endl;

    // Initialize output-owning controllers first so outputs are immediately
    // configured and forced to their safe OFF states.
    safety.begin();
    winch.begin();

    initializeLights(lightController);

    safety.safeStartup();

    initializeButtons(lightController);

    cout << F("System Ready") << endl;
}

void loop() {
    // Update all buttons.
    for (auto button : allButtons) {
        button->update();
    }

    // Winch is hold-to-run.
    // If both direction buttons are pressed, command STOP.
    const bool winchUpPressed = winchUpButton->isPressed();
    const bool winchDownPressed = winchDownButton->isPressed();

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
