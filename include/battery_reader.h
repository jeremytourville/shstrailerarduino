#pragma once

namespace shstrailer {

class BatteryReader {
   public:
    virtual ~BatteryReader() = default;

    [[nodiscard]] virtual float readVoltage() { return 0.0f; }
};

}  // namespace shstrailer
