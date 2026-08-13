#pragma once

#include "types.h"

namespace shstrailer {

class BatteryObserver {
   public:
    virtual ~BatteryObserver() = default;

    virtual void onBatteryVoltage([[maybe_unused]] float voltage) {}

    virtual void onBatteryState([[maybe_unused]] BatteryState state) {}
};

}  // namespace shstrailer
