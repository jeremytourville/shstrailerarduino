#pragma once

#include <Arduino.h>

#include "button_observer.hpp"

namespace shstrailer {

class Light : public ButtonObserver {
   public:
    explicit Light(uint8_t pin);

    void onPressed(uint8_t pin) override;

    void on();

    void off();

    [[nodiscard]] uint8_t getPin() const;

   private:
    void write(uint8_t newState);

    const uint8_t pin_;
    uint8_t state_ = LOW;
};

}  // namespace shstrailer
