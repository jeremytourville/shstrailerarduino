#include "avr_battery_reader.h"

#include <stdint.h>

#include "pins.h"

namespace shstrailer {

namespace {

constexpr uint8_t kAverageSamples = 8;
constexpr float kADCReferenceVoltage = 5.0f;
constexpr uint16_t kADCMaxCounts = 1023;
constexpr float kVoltageDividerR1 = 100000.0f;
constexpr float kVoltageDividerR2 = 33000.0f;

}  // namespace

float AVRBatteryReader::readVoltage() {
    uint32_t adcTotal = 0;

    for (uint8_t i = 0; i < kAverageSamples; ++i) {
        adcTotal += analogRead(BATTERY_VOLTAGE_PIN);
    }

    const float adcAverage = (float)adcTotal / (float)kAverageSamples;

    const float vPin =
        (adcAverage * kADCReferenceVoltage) / (float)kADCMaxCounts;

    const float dividerRatio =
        (kVoltageDividerR1 + kVoltageDividerR2) / kVoltageDividerR2;

    return vPin * dividerRatio;
}

}  // namespace shstrailer
