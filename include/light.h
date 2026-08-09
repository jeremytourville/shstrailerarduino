#pragma once

#include <Arduino.h>

#include "button_observer.hpp"

namespace shstrailer {

class Light : public ButtonObserver {
   public:
    explicit Light(uint8_t pin);

    void onPressed() override;

    void on();

    void off();

   private:
    void write(uint8_t newState);

    static constexpr uint8_t kUninitialized = 3;

    const uint8_t pin_;
    // set to a state that is neither HIGH nor LOW so that the first call to
    // write() calls pinMode()
    uint8_t state_ = kUninitialized;
};

}  // namespace shstrailer
