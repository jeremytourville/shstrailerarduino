#pragma once

#include <stdint.h>

namespace shstrailer {

class ButtonObserver {
   public:
    virtual ~ButtonObserver() = default;

    virtual void onPressed([[maybe_unused]] uint8_t pin) {};

    virtual void onLongPressed([[maybe_unused]] uint8_t pin) {};
};

}  // namespace shstrailer
