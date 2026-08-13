#pragma once

#include <stdint.h>

#include "config.h"

namespace shstrailer {

class EndLine {};

constexpr inline EndLine endl;

class Console {
   public:
    friend Console& operator<<(Console& console, const char value);

    friend Console& operator<<(Console& console, const uint8_t value);

    friend Console& operator<<(Console& console, const int16_t value);

    friend Console& operator<<(Console& console, const uint16_t value);

    friend Console& operator<<(Console& console, const int32_t value);

    friend Console& operator<<(Console& console, const uint32_t value);

    friend Console& operator<<(Console& console, const float value);

    friend Console& operator<<(Console& console, const double value);

    friend Console& operator<<(Console& console, const char* const value);

    friend Console& operator<<(Console& console,
                               const __FlashStringHelper* const value);

    friend Console& operator<<(Console& console, const EndLine&);

   private:
    bool initialized_ = false;
};

inline Console cout;

}  // namespace shstrailer
