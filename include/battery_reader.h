#pragma once

namespace shstrailer {

class BatteryReader {
   public:
    virtual ~BatteryReader() = default;

    [[nodiscard]] virtual float readVoltage() = 0;
};

}  // namespace shstrailer
