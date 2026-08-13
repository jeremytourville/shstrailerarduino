#pragma once

#include <stdint.h>

#include "observers/battery_observer.h"
#include "timer.h"
#include "types.h"
#include "vector.h"

namespace shstrailer {

class Battery {
   public:
    void update();

    void registerObserver(BatteryObserver* observer);

   private:
    void readVoltage();

    void notifyVoltage();

    void notifyState();

    float voltage_ = 0.0f;
    BatteryState state_ = BatteryState::OK;
    BatteryState potentialState_ = BatteryState::OK;
    Timer sampleTimer_;
    Timer stateTimer_;
    Vector<BatteryObserver*, 2> observers_;
};

}  // namespace shstrailer
