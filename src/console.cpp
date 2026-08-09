#include "console.h"

namespace shstrailer {

namespace {

template <typename T>
void PrintToSerial(bool& initialized, const T& value) {
    if (!ENABLE_SERIAL_DEBUG) {
        return;
    }

    if (!initialized) {
        initialized = true;
        Serial.begin(SERIAL_BAUD);
    }

    Serial.print(value);
}

}  // namespace

Console& operator<<(Console& console, const int8_t value) {
    PrintToSerial(console.initialized_, value);

    return console;
}

Console& operator<<(Console& console, const uint8_t value) {
    PrintToSerial(console.initialized_, value);

    return console;
}

Console& operator<<(Console& console, const int16_t value) {
    PrintToSerial(console.initialized_, value);

    return console;
}

Console& operator<<(Console& console, const uint16_t value) {
    PrintToSerial(console.initialized_, value);

    return console;
}

Console& operator<<(Console& console, const int32_t value) {
    PrintToSerial(console.initialized_, value);

    return console;
}

Console& operator<<(Console& console, const uint32_t value) {
    PrintToSerial(console.initialized_, value);

    return console;
}

Console& operator<<(Console& console, const float value) {
    PrintToSerial(console.initialized_, value);

    return console;
}

Console& operator<<(Console& console, const double value) {
    PrintToSerial(console.initialized_, value);

    return console;
}

Console& operator<<(Console& console, const char* const value) {
    PrintToSerial(console.initialized_, value);

    return console;
}

Console& operator<<(Console& console, const __FlashStringHelper* const value) {
    PrintToSerial(console.initialized_, value);

    return console;
}

Console& operator<<(Console& console, const EndLine&) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.println();
    }

    return console;
}

}  // namespace shstrailer
