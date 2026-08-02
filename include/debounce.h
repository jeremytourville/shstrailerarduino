#pragma once

#include <Arduino.h>

#include "config.h"

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
    uint32_t m_lastDebounceTime;
};

}  // namespace shstrailer
