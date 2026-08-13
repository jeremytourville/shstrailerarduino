#pragma once

#include "battery_reader.h"
#include "timer.h"

namespace shstrailer {

class MockBatteryReader : public BatteryReader {
   public:
    [[nodiscard]] float readVoltage() override;

   private:
    Timer simulationTimer_;
    float delta_ = 0.0f;
    float voltage_ = 12.5f;
};

}  // namespace shstrailer
