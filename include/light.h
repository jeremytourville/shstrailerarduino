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

    const uint8_t pin_;
    uint8_t state_ = LOW;
};

}  // namespace shstrailer
