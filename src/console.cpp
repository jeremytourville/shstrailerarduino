#include "console.h"

namespace shstrailer {

Console& operator<<(Console& console, const int8_t value) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.print(value);
    }

    return console;
}

Console& operator<<(Console& console, const uint8_t value) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.print(value);
    }

    return console;
}

Console& operator<<(Console& console, const int16_t value) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.print(value);
    }

    return console;
}

Console& operator<<(Console& console, const uint16_t value) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.print(value);
    }

    return console;
}

Console& operator<<(Console& console, const int32_t value) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.print(value);
    }

    return console;
}

Console& operator<<(Console& console, const uint32_t value) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.print(value);
    }

    return console;
}

Console& operator<<(Console& console, const float value) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.print(value);
    }

    return console;
}

Console& operator<<(Console& console, const double value) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.print(value);
    }

    return console;
}

Console& operator<<(Console& console, const char* const str) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.print(str);
    }

    return console;
}

Console& operator<<(Console& console, const __FlashStringHelper* const str) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.print(str);
    }

    return console;
}

Console& operator<<(Console& console, const EndLine&) {
    if (ENABLE_SERIAL_DEBUG) {
        Serial.println();
    }

    return console;
}

Console::Console() { Serial.begin(SERIAL_BAUD); }

}  // namespace shstrailer
