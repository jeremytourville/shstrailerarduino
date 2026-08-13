#pragma once

#include <stdint.h>

#include "battery_reader.h"
#include "observers/battery_observer.h"
#include "timer.h"
#include "types.h"
#include "vector.h"

namespace shstrailer {

class Battery {
   public:
    explicit Battery(BatteryReader& reader);

    void update();

    void registerObserver(BatteryObserver* observer);

   private:
    void notifyVoltage();

    void notifyState();

    float voltage_ = 0.0f;
    BatteryState state_ = BatteryState::OK;
    BatteryState potentialState_ = BatteryState::OK;
    Timer sampleTimer_;
    Timer stateTimer_;
    Vector<BatteryObserver*, 2> observers_;
    BatteryReader& reader_;
};

}  // namespace shstrailer
