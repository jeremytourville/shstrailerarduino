#pragma once

#include <DIYables_OLED_SSD1309.h>
#include <stdint.h>

#include "observers/heartbeat_observer.h"
#include "observers/light_observer.h"
#include "observers/winch_observer.h"
#include "timer.h"
#include "version.h"

namespace shstrailer {

class Screen : public LightObserver,
               public WinchObserver,
               public HeartBeatObserver {
   public:
    Screen();

    void onLightState(uint8_t pin, uint8_t state) override;

    void onWinchState(WinchState state,
                      Timer::Duration cooldownTimeRemaining) override;

    void onHeartBeat() override;

    void update();

   private:
    [[nodiscard]] bool initialize();

    void drawText(int16_t x, int16_t y, const char* text, uint8_t textSize = 1,
                  uint16_t color = SSD1309_PIXEL_ON);

    void drawLightsGroup();

    void drawWinchGroup();

    void drawCoolDownTimeRemaining();

    void drawStatusBar();

    DIYables_OLED_SSD1309 display_;
    bool initialized_ = false;
    char versionString_[kVersionBufferSize];

    // 7 comes from sign, two digits, decimal point, two digits, null terminator
    static constexpr int kVoltageBufferSize = 7;
    char voltageString_[kVoltageBufferSize];
    int16_t bottomHLineY_ = 0;
    int16_t versionX_ = 0;
    int16_t versionY_ = 0;
    bool drawHeartbeat_ = false;
    WinchState winchState_ = WinchState::IDLE;
    Timer::Duration winchCooldownTimeRemaining_ = 0;
    Timer timer_;
    static constexpr int8_t kLight1Mask = 0x01;
    static constexpr int8_t kLight2Mask = 0x02;
    static constexpr int8_t kLight3Mask = 0x04;
    static constexpr int8_t kLight4Mask = 0x08;
    static constexpr int8_t kDualPodLightsMask = 0x10;
    static constexpr int8_t kLEDStripLightMask = 0x20;
    int8_t lightStates_ = 0;
};

}  // namespace shstrailer
