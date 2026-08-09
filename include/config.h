#pragma once

#include <Arduino.h>

#include "timer.h"

#define FW_NAME "Trailer Controller"

namespace shstrailer {

constexpr bool ENABLE_SERIAL_DEBUG = true;
constexpr bool ENABLE_BATTERY_MONITOR = true;
constexpr bool ENABLE_STATUS_LED = true;

// Input filtering
constexpr Timer::Stamp BUTTON_DEBOUNCE_MS = 30;

// Winch protection
constexpr Timer::Stamp WINCH_DIRECTION_DELAY_MS = 250;

// Manufacturer duty-cycle limit:
// Maximum continuous run: 45 seconds.
// 5% duty cycle means 19 seconds OFF for every 1 second ON.
// Therefore, a full 45-second run requires 855 seconds (14 min 15 sec) OFF.
constexpr Timer::Stamp WINCH_MAX_CONTINUOUS_RUNTIME_MS = 45000UL;
constexpr uint8_t WINCH_DUTY_PERCENT = 5;
constexpr uint8_t WINCH_OFF_TO_ON_RATIO =
    (100U - WINCH_DUTY_PERCENT) / WINCH_DUTY_PERCENT;

// Startup
constexpr Timer::Stamp STARTUP_LOCKOUT_MS = 500;

// Battery monitor
constexpr Timer::Stamp BATTERY_SAMPLE_INTERVAL_MS = 250;
constexpr uint8_t BATTERY_AVERAGE_SAMPLES = 8;

constexpr float ADC_REFERENCE_VOLTAGE = 5.0f;
constexpr uint16_t ADC_MAX_COUNTS = 1023;

constexpr float VOLTAGE_DIVIDER_R1 = 100000.0f;
constexpr float VOLTAGE_DIVIDER_R2 = 33000.0f;

constexpr float BATTERY_WARNING_VOLT = 12.20f;
constexpr float BATTERY_CRITICAL_VOLT = 11.80f;

// Logic levels
constexpr bool OUTPUT_ON = HIGH;
constexpr bool OUTPUT_OFF = LOW;

}  // namespace shstrailer
