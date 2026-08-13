#pragma once

#include <Arduino.h>

#include "observers/button_observer.h"

namespace shstrailer {

class Light : public ButtonObserver {
   public:
    explicit Light(uint8_t pin);

    void onButtonPressed(uint8_t pin) override;

    void on();

    void off();

    [[nodiscard]] uint8_t getPin() const;

   private:
    void write(uint8_t newState);

    const uint8_t pin_;
    uint8_t state_ = LOW;
};

}  // namespace shstrailer
