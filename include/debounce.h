#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <Arduino.h>
#include "config.h"

class DebouncedButton {
public:
    explicit DebouncedButton(uint8_t pin);

    void begin();
    void update();

    bool wasPressed();
    bool isPressed() const;

private:
    uint8_t m_pin;
    bool m_currentState;
    bool m_previousState;
    bool m_pressEvent;
    uint32_t m_lastDebounceTime;
};

#endif
