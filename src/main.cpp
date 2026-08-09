#include <Arduino.h>

#include <new>

#include "button.h"
#include "config.h"
#include "console.h"
#include "frame_rate_monitor.h"
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

Vector<Button*> allButtons;

//
// Lights
//
ObjectAllocator<Light, 6> lightAllocator;

//
// Controllers
//
LightController lightController;
WinchController winch;
SafetyController safety;

FrameRateMonitor frameRateMonitor;

void initializeLights(LightController& lightControllerLocal) {
    lightControllerLocal.registerLight(lightAllocator.allocate(LIGHT1_OUT));
    lightControllerLocal.registerLight(lightAllocator.allocate(LIGHT2_OUT));
    lightControllerLocal.registerLight(lightAllocator.allocate(LIGHT3_OUT));
    lightControllerLocal.registerLight(lightAllocator.allocate(LIGHT4_OUT));
    lightControllerLocal.registerLight(lightAllocator.allocate(LED_STRIP_OUT));
    lightControllerLocal.registerLight(lightAllocator.allocate(POD_LIGHT_OUT));
}

void initializeButtons(LightController& lightControllerLocal,
                       WinchController& winchLocal) {
    allButtons.push_back(buttonAllocator.allocate(WINCH_UP_SW));
    allButtons.back()->registerObserver(&winchLocal);

    allButtons.push_back(buttonAllocator.allocate(WINCH_DN_SW));
    allButtons.back()->registerObserver(&winchLocal);

    // map buttons to lights
    Light* light1 = lightControllerLocal.getLightByPin(LIGHT1_OUT);
    allButtons.push_back(buttonAllocator.allocate(L1_SW_A));
    allButtons.back()->registerObserver(light1);
    allButtons.push_back(buttonAllocator.allocate(L1_SW_B));
    allButtons.back()->registerObserver(light1);

    Light* light2 = lightControllerLocal.getLightByPin(LIGHT2_OUT);
    allButtons.push_back(buttonAllocator.allocate(L2_SW_A));
    allButtons.back()->registerObserver(light2);
    allButtons.push_back(buttonAllocator.allocate(L2_SW_B));
    allButtons.back()->registerObserver(light2);

    Light* light3 = lightControllerLocal.getLightByPin(LIGHT3_OUT);
    allButtons.push_back(buttonAllocator.allocate(L3_SW_A));
    allButtons.back()->registerObserver(light3);
    allButtons.push_back(buttonAllocator.allocate(L3_SW_B));
    allButtons.back()->registerObserver(light3);

    Light* light4 = lightControllerLocal.getLightByPin(LIGHT4_OUT);
    allButtons.push_back(buttonAllocator.allocate(L4_SW_A));
    allButtons.back()->registerObserver(light4);
    allButtons.push_back(buttonAllocator.allocate(L4_SW_B));
    allButtons.back()->registerObserver(light4);

    Light* ledStrip = lightControllerLocal.getLightByPin(LED_STRIP_OUT);
    allButtons.push_back(buttonAllocator.allocate(LED_STRIP_SW));
    allButtons.back()->registerObserver(ledStrip);

    Light* podLight = lightControllerLocal.getLightByPin(POD_LIGHT_OUT);
    allButtons.push_back(buttonAllocator.allocate(POD_LIGHT_SW));
    allButtons.back()->registerObserver(podLight);

    // register the light controller to turn off all lights on long press
    for (auto button : allButtons) {
        button->registerObserver(&lightControllerLocal);
    }
}

void setup() {
    cout << endl << FW_NAME << F(" ") << GetVersionString() << endl;

    // Initialize output-owning controllers first so outputs are immediately
    // configured and forced to their safe OFF states.
    safety.begin();
    winch.begin();

    initializeLights(lightController);

    safety.safeStartup();

    initializeButtons(lightController, winch);

    cout << F("System Ready") << endl;
}

void loop() {
    // Update all buttons.
    for (auto button : allButtons) {
        button->update();
    }

    winch.update();

    safety.setWinchFault(winch.isFaulted());
    safety.setWinchCooldown(winch.isCoolingDown());
    safety.update();

    frameRateMonitor.update();
}
