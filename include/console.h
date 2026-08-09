#pragma once

#include <Arduino.h>

#include "config.h"

namespace shstrailer {

class EndLine {};

inline EndLine endl;

class Console {
   public:
    Console();

    friend Console& operator<<(Console& console, const int8_t value);

    friend Console& operator<<(Console& console, const uint8_t value);

    friend Console& operator<<(Console& console, const int16_t value);

    friend Console& operator<<(Console& console, const uint16_t value);

    friend Console& operator<<(Console& console, const int32_t value);

    friend Console& operator<<(Console& console, const uint32_t value);

    friend Console& operator<<(Console& console, const float value);

    friend Console& operator<<(Console& console, const double value);

    friend Console& operator<<(Console& console,
                               const __FlashStringHelper* const str);

    friend Console& operator<<(Console& console, const EndLine&);
};

inline Console cout;

}  // namespace shstrailer
