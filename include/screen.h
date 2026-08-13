#pragma once

#include <DIYables_OLED_SSD1309.h>
#include <stdint.h>

#include "observers/battery_observer.h"
#include "observers/heartbeat_observer.h"
#include "observers/winch_observer.h"
#include "timer.h"
#include "version.h"

namespace shstrailer {

class Screen : public BatteryObserver,
               public WinchObserver,
               public HeartBeatObserver {
   public:
    Screen();

    void onBatteryVoltage(float voltage) override;

    void onBatteryState(BatteryState state) override;

    void onWinchState(WinchState state,
                      Timer::Stamp cooldownTimeRemaining) override;

    void onHeartBeat() override;

    void beginDisplay();

    void endDisplay();

   private:
    [[nodiscard]] bool initialize();

    [[nodiscard]] const char* voltageToString();

    void drawText(int16_t x, int16_t y, const char* text, uint8_t textSize = 1,
                  uint16_t color = SSD1309_PIXEL_ON);

    void drawBattery(int16_t x, int16_t y);

    void drawBatteryGroup();

    void drawWinchGroup();

    void drawCoolDownTimeRemaining();

    void drawStatusBar();

    DIYables_OLED_SSD1309 display_;
    bool initialized_ = false;
    bool available_ = false;
    char versionString_[kVersionBufferSize];

    // 7 comes from sign, two digits, decimal point, two digits, null terminator
    static constexpr int kVoltageBufferSize = 7;
    char voltageString_[kVoltageBufferSize];
    int16_t bottomHLineY_ = 0;
    int16_t versionX_ = 0;
    int16_t versionY_ = 0;
    BatteryState batteryState_ = BatteryState::OK;
    float batteryVoltage_ = 0.0f;
    bool drawHeartbeat_ = false;
    WinchState winchState_;
    Timer::Stamp winchCooldownTimeRemaining_;
    Timer timer_;
};

}  // namespace shstrailer
