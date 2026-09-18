#include <Arduino.h>
#include <avr/wdt.h>

#include "button.h"
#include "console.h"
#include "frame_rate_monitor.h"
#include "heartbeat.h"
#include "light_controller.h"
#include "pins.h"
#include "screen.h"
#include "status_led.h"
#include "vector.h"
#include "version.h"
#include "winch.h"

using namespace shstrailer;

//
// Buttons
//
Vector<Button, 12> allButtons;

//
// Controllers
//
LightController lightController;
Winch winch;
HeartBeat heartBeat;
StatusLED statusLED;

FrameRateMonitor frameRateMonitor;
Screen screen;

void initializeLights(LightController& lightControllerLocal) {
    lightControllerLocal.registerLight(LIGHT1_OUT);
    lightControllerLocal.registerLight(LIGHT2_OUT);
    lightControllerLocal.registerLight(LIGHT3_OUT);
    lightControllerLocal.registerLight(LIGHT4_OUT);
    lightControllerLocal.registerLight(LED_STRIP_OUT);
    lightControllerLocal.registerLight(POD_LIGHT_OUT);
}

void initializeButtonsAndObservers(LightController& lightControllerLocal,
                                   Winch& winchLocal) {
    // map buttons to lights
    Light* light1 = lightControllerLocal.getLightByPin(LIGHT1_OUT);
    allButtons.emplace_back(L1_SW_A);
    allButtons.back().registerObserver(light1);
    allButtons.emplace_back(L1_SW_B);
    allButtons.back().registerObserver(light1);
    light1->registerObserver(&screen);

    Light* light2 = lightControllerLocal.getLightByPin(LIGHT2_OUT);
    allButtons.emplace_back(L2_SW_A);
    allButtons.back().registerObserver(light2);
    allButtons.emplace_back(L2_SW_B);
    allButtons.back().registerObserver(light2);
    light2->registerObserver(&screen);

    Light* light3 = lightControllerLocal.getLightByPin(LIGHT3_OUT);
    allButtons.emplace_back(L3_SW_A);
    allButtons.back().registerObserver(light3);
    allButtons.emplace_back(L3_SW_B);
    allButtons.back().registerObserver(light3);
    light3->registerObserver(&screen);

    Light* light4 = lightControllerLocal.getLightByPin(LIGHT4_OUT);
    allButtons.emplace_back(L4_SW_A);
    allButtons.back().registerObserver(light4);
    allButtons.emplace_back(L4_SW_B);
    allButtons.back().registerObserver(light4);
    light4->registerObserver(&screen);

    Light* ledStrip = lightControllerLocal.getLightByPin(LED_STRIP_OUT);
    allButtons.emplace_back(LED_STRIP_SW);
    allButtons.back().registerObserver(ledStrip);
    ledStrip->registerObserver(&screen);

    Light* podLight = lightControllerLocal.getLightByPin(POD_LIGHT_OUT);
    allButtons.emplace_back(POD_LIGHT_SW);
    allButtons.back().registerObserver(podLight);
    podLight->registerObserver(&screen);

    // Register the light controller to turn off all lights on long press,
    // exclude winch buttons by registering them after this loop.
    for (auto& button : allButtons) {
        button.registerObserver(&lightControllerLocal);
    }

    allButtons.emplace_back(WINCH_UP_SW);
    allButtons.back().registerObserver(&winchLocal);

    allButtons.emplace_back(WINCH_DN_SW);
    allButtons.back().registerObserver(&winchLocal);
}

void setup() {
    // Reset MCU if loop() hangs >8s.
    wdt_enable(WDTO_8S);

    cout << endl << F("Trailer Controller ") << GetVersionString() << endl;

    // Initialize output-owning controllers first so outputs are immediately
    // configured and forced to their safe OFF states.
    winch.begin();

    initializeLights(lightController);

    initializeButtonsAndObservers(lightController, winch);

    heartBeat.registerObserver(&screen);
    heartBeat.registerObserver(&statusLED);

    winch.registerObserver(&screen);
    winch.registerObserver(&statusLED);

    // set tubivator pin always on
    pinMode(TUBIVATOR_OUT, OUTPUT);
    digitalWrite(TUBIVATOR_OUT, HIGH);

    cout << F("System Ready") << endl;
}

void loop() {
    wdt_reset();

    // Update all buttons.
    for (auto& button : allButtons) {
        button.update();
    }

    winch.update();
    heartBeat.update();
    statusLED.update();
    screen.update();

    frameRateMonitor.update();
}
