#pragma once

#include <Arduino.h>

#include "config.h"
#include "safe_timer.h"

namespace shstrailer {

class DebouncedButton {
   public:
    explicit DebouncedButton(uint8_t pin);

    void begin();

    void update();

    [[nodiscard]] bool wasPressed();

    [[nodiscard]] bool isPressed() const;

   private:
    const uint8_t m_pin;
    bool m_currentState;
    bool m_previousState;
    bool m_pressEvent;

    SafeTimer m_timer;
};

}  // namespace shstrailer
