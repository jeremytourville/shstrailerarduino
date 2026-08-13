#pragma once

#include <Arduino.h>
#include <limits.h>

#include "observers/battery_observer.h"
#include "observers/heartbeat_observer.h"
#include "observers/winch_observer.h"
#include "timer.h"

namespace shstrailer {

class StatusLED : public HeartBeatObserver,
                  public WinchObserver,
                  public BatteryObserver {
   public:
    void update();

    void onHeartBeat() override;

    void onWinchState(WinchState state,
                      Timer::Duration cooldownTimeRemaining) override;

    void onBatteryState(BatteryState state) override;

   private:
    void toggle();

    Timer timer_;
    bool initialized_ = false;
    uint8_t state_ = LOW;
    Timer::Duration winchDelay_ = ULONG_MAX;
    Timer::Duration batteryDelay_ = ULONG_MAX;
};

}  // namespace shstrailer
