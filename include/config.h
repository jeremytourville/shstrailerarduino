#pragma once

#include <Arduino.h>

#define FW_NAME "Trailer Controller"

namespace shstrailer {

constexpr bool ENABLE_SERIAL_DEBUG = true;
constexpr bool ENABLE_BATTERY_MONITOR = true;
constexpr bool ENABLE_STATUS_LED = true;

// Input filtering
constexpr uint16_t BUTTON_DEBOUNCE_MS = 30;

// Winch protection
constexpr uint16_t WINCH_DIRECTION_DELAY_MS = 250;

// Manufacturer duty-cycle limit:
// Maximum continuous run: 45 seconds.
// 5% duty cycle means 19 seconds OFF for every 1 second ON.
// Therefore, a full 45-second run requires 855 seconds (14 min 15 sec) OFF.
constexpr uint32_t WINCH_MAX_CONTINUOUS_RUNTIME_MS = 45000UL;
constexpr uint8_t WINCH_DUTY_PERCENT = 5;
constexpr uint8_t WINCH_OFF_TO_ON_RATIO =
    (100U - WINCH_DUTY_PERCENT) / WINCH_DUTY_PERCENT;

// Startup
constexpr uint16_t STARTUP_LOCKOUT_MS = 500;

// Battery monitor
constexpr uint16_t BATTERY_SAMPLE_INTERVAL_MS = 250;
constexpr uint8_t BATTERY_AVERAGE_SAMPLES = 8;

constexpr float ADC_REFERENCE_VOLTAGE = 5.0f;
constexpr uint16_t ADC_MAX_COUNTS = 1023;

constexpr float VOLTAGE_DIVIDER_R1 = 100000.0f;
constexpr float VOLTAGE_DIVIDER_R2 = 33000.0f;

constexpr float BATTERY_WARNING_VOLT = 12.20f;
constexpr float BATTERY_CRITICAL_VOLT = 11.80f;

// Serial
constexpr uint32_t SERIAL_BAUD_RATE = 115200;

// Logic levels
constexpr bool SWITCH_PRESSED = LOW;
constexpr bool SWITCH_RELEASED = HIGH;

constexpr bool OUTPUT_ON = HIGH;
constexpr bool OUTPUT_OFF = LOW;

}  // namespace shstrailer
