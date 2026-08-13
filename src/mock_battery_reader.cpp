#include "mock_battery_reader.h"

#include <stdlib.h>

#include "console.h"

namespace shstrailer {

namespace {

[[nodiscard]] float noise() {
    return (rand() / static_cast<float>(RAND_MAX)) / 100.0f;
}

}  // namespace

float MockBatteryReader::readVoltage() {
    if (simulationTimer_.elapsed() < 60000) {
        // simulate an ok battery
        delta_ = 0.0f;
    } else if (simulationTimer_.elapsed() < 180000) {
        // simulate a draining battery
        delta_ = -0.001f;
    } else if (simulationTimer_.elapsed() < 240000) {
        // simulate a draining battery
        delta_ = -0.002f;
    } else if (simulationTimer_.elapsed() < 300000) {
        // simulate a charging battery
        delta_ = 0.0015f;
    }

    voltage_ += delta_;

    return voltage_ + noise();
}

}  // namespace shstrailer
