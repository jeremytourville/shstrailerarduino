#pragma once

#include "battery_reader.h"

namespace shstrailer {

class AVRBatteryReader : public BatteryReader {
   public:
    [[nodiscard]] float readVoltage() override;
};

}  // namespace shstrailer
