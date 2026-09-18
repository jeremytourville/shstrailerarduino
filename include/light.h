#pragma once

#include <Arduino.h>

#include "observers/button_observer.h"
#include "observers/light_observer.h"
#include "vector.h"

namespace shstrailer {

class Light : public ButtonObserver {
   public:
    explicit Light(uint8_t pin);

    void onButtonPressed(uint8_t pin) override;

    void on();

    void off();

    [[nodiscard]] uint8_t getPin() const;

    void registerObserver(LightObserver* observer);

   private:
    void write(uint8_t newState);

    void notify();

    Vector<LightObserver*, 2> observers_;
    const uint8_t pin_;
    uint8_t state_ = LOW;
};

}  // namespace shstrailer
