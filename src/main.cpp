#include <Arduino.h>

#include "avr_battery_reader.h"
#include "battery.h"
#include "button.h"
#include "console.h"
#include "frame_rate_monitor.h"
#include "heartbeat.h"
#include "light_controller.h"
#include "mock_battery_reader.h"
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
WinchController winch;
AVRBatteryReader batteryReader;
// MockBatteryReader batteryReader;
Battery battery(batteryReader);
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

void initializeButtons(LightController& lightControllerLocal,
                       WinchController& winchLocal) {
    allButtons.emplace_back(WINCH_UP_SW);
    allButtons.back().registerObserver(&winchLocal);

    allButtons.emplace_back(WINCH_DN_SW);
    allButtons.back().registerObserver(&winchLocal);

    // map buttons to lights
    Light* light1 = lightControllerLocal.getLightByPin(LIGHT1_OUT);
    allButtons.emplace_back(L1_SW_A);
    allButtons.back().registerObserver(light1);
    allButtons.emplace_back(L1_SW_B);
    allButtons.back().registerObserver(light1);

    Light* light2 = lightControllerLocal.getLightByPin(LIGHT2_OUT);
    allButtons.emplace_back(L2_SW_A);
    allButtons.back().registerObserver(light2);
    allButtons.emplace_back(L2_SW_B);
    allButtons.back().registerObserver(light2);

    Light* light3 = lightControllerLocal.getLightByPin(LIGHT3_OUT);
    allButtons.emplace_back(L3_SW_A);
    allButtons.back().registerObserver(light3);
    allButtons.emplace_back(L3_SW_B);
    allButtons.back().registerObserver(light3);

    Light* light4 = lightControllerLocal.getLightByPin(LIGHT4_OUT);
    allButtons.emplace_back(L4_SW_A);
    allButtons.back().registerObserver(light4);
    allButtons.emplace_back(L4_SW_B);
    allButtons.back().registerObserver(light4);

    Light* ledStrip = lightControllerLocal.getLightByPin(LED_STRIP_OUT);
    allButtons.emplace_back(LED_STRIP_SW);
    allButtons.back().registerObserver(ledStrip);

    Light* podLight = lightControllerLocal.getLightByPin(POD_LIGHT_OUT);
    allButtons.emplace_back(POD_LIGHT_SW);
    allButtons.back().registerObserver(podLight);

    // register the light controller to turn off all lights on long press
    for (auto& button : allButtons) {
        button.registerObserver(&lightControllerLocal);
    }
}

void setup() {
    cout << endl << FW_NAME << F(" ") << GetVersionString() << endl;

    // Initialize output-owning controllers first so outputs are immediately
    // configured and forced to their safe OFF states.
    winch.begin();

    initializeLights(lightController);

    initializeButtons(lightController, winch);

    battery.registerObserver(&screen);
    battery.registerObserver(&statusLED);

    heartBeat.registerObserver(&screen);
    heartBeat.registerObserver(&statusLED);

    winch.registerObserver(&screen);
    winch.registerObserver(&statusLED);

    cout << F("System Ready") << endl;
}

void loop() {
    // Update all buttons.
    for (auto& button : allButtons) {
        button.update();
    }

    winch.update();
    battery.update();
    heartBeat.update();
    statusLED.update();
    screen.update();

    frameRateMonitor.update();
}
