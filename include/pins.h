#pragma once

#include <stdint.h>

namespace shstrailer {

constexpr uint8_t STATUS_LED = 13;

constexpr uint8_t L1_SW_A = 22;
constexpr uint8_t L1_SW_B = 23;
constexpr uint8_t L2_SW_A = 24;
constexpr uint8_t L2_SW_B = 25;
constexpr uint8_t L3_SW_A = 26;
constexpr uint8_t L3_SW_B = 27;
constexpr uint8_t L4_SW_A = 28;
constexpr uint8_t L4_SW_B = 29;

constexpr uint8_t LED_STRIP_SW = 30;
constexpr uint8_t POD_LIGHT_SW = 31;

constexpr uint8_t WINCH_UP_SW = 32;
constexpr uint8_t WINCH_DN_SW = 33;

constexpr uint8_t LIGHT1_OUT = 34;
constexpr uint8_t LIGHT2_OUT = 35;
constexpr uint8_t LIGHT3_OUT = 36;
constexpr uint8_t LIGHT4_OUT = 37;
constexpr uint8_t LED_STRIP_OUT = 38;
constexpr uint8_t POD_LIGHT_OUT = 39;
constexpr uint8_t WINCH_UP_OUT = 40;
constexpr uint8_t WINCH_DN_OUT = 41;

constexpr uint8_t BATTERY_VOLTAGE_PIN = A0;

}  // namespace shstrailer
