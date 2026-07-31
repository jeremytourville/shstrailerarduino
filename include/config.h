#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

#define FW_NAME "Trailer Controller"

constexpr bool ENABLE_SERIAL_DEBUG = true;
constexpr bool ENABLE_BATTERY_MONITOR = true;
constexpr bool ENABLE_STATUS_LED = true;

constexpr uint16_t BUTTON_DEBOUNCE_MS = 30;
constexpr uint16_t WINCH_DIRECTION_DELAY = 250;
constexpr uint16_t STARTUP_LOCKOUT_MS = 500;
constexpr uint32_t WINCH_MAX_RUNTIME_MS = 30000UL;

constexpr float ADC_REFERENCE_VOLTAGE = 5.0f;
constexpr uint16_t ADC_MAX_COUNTS = 1023;
constexpr float VOLTAGE_DIVIDER_R1 = 100000.0f;
constexpr float VOLTAGE_DIVIDER_R2 = 33000.0f;
constexpr float BATTERY_WARNING_VOLT = 12.20f;
constexpr float BATTERY_CRITICAL_VOLT = 11.80f;

constexpr uint32_t SERIAL_BAUD_RATE = 115200;

constexpr bool SWITCH_PRESSED = LOW;
constexpr bool SWITCH_RELEASED = HIGH;
constexpr bool OUTPUT_ON = HIGH;
constexpr bool OUTPUT_OFF = LOW;

#endif
