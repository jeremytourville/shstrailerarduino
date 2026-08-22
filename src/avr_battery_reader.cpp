#include "avr_battery_reader.h"

#include <Arduino.h>
#include <stdint.h>

#include "median.h"
#include "pins.h"

namespace shstrailer {

namespace {

constexpr uint8_t kAverageSamples = 8;
constexpr float kADCReferenceVoltage = 5.0f;
constexpr uint16_t kADCMaxCounts = 1023;
constexpr float kVoltageDividerR1 = 100000.0f;
constexpr float kVoltageDividerR2 = 33000.0f;

// Wait longer than ATmega2560 ADC acquisition time (~13 ADC clocks)
// between samples so each conversion sees the true divider value.
constexpr uint8_t kAdcSettlingDelayUs = 100;

}  // namespace

float AVRBatteryReader::readVoltage() {
    // Settle the reference. DEFAULT == AVcc, matches the ADC's actual
    // conversion reference on the Mega.
    analogReference(DEFAULT);

    // ignore the first sample, to allow capacitors and MUXs settle.
    analogRead(BATTERY_VOLTAGE_PIN);

    uint32_t samples[kAverageSamples];

    for (uint8_t i = 0; i < kAverageSamples; ++i) {
        delayMicroseconds(kAdcSettlingDelayUs);
        samples[i] = analogRead(BATTERY_VOLTAGE_PIN);
    }

    uint32_t filtered[kAverageSamples];
    medianFilter(filtered, samples);

    uint32_t adcTotal = 0;

    for (uint8_t i = 0; i < kAverageSamples; ++i) {
        adcTotal += filtered[i];
    }

    const float adcAverage =
        static_cast<float>(adcTotal) / static_cast<float>(kAverageSamples);

    const float vPin =
        (adcAverage * kADCReferenceVoltage) / static_cast<float>(kADCMaxCounts);

    const float dividerRatio =
        (kVoltageDividerR1 + kVoltageDividerR2) / kVoltageDividerR2;

    return vPin * dividerRatio;
}

}  // namespace shstrailer
