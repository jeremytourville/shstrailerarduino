#include "debounce.h"

namespace shstrailer {

DebouncedButton::DebouncedButton(const uint8_t pin)
    : m_pin(pin),
      m_currentState(HIGH),
      m_previousState(HIGH),
      m_pressEvent(false) {}

void DebouncedButton::begin() {
    pinMode(m_pin, INPUT_PULLUP);
    m_currentState = digitalRead(m_pin);
    m_previousState = m_currentState;
    m_timer.start();
}

void DebouncedButton::update() {
    const bool raw = digitalRead(m_pin);

    if (raw != m_previousState) {
        m_timer.start();
        m_previousState = raw;
    }

    if (m_timer.elapsed() >= BUTTON_DEBOUNCE_MS) {
        if (raw != m_currentState) {
            m_currentState = raw;
            if (m_currentState == SWITCH_PRESSED) {
                m_pressEvent = true;
            }
        }
    }
}

bool DebouncedButton::wasPressed() {
    const bool pressEventLocal = m_pressEvent;
    m_pressEvent = false;

    return pressEventLocal;
}

bool DebouncedButton::isPressed() const {
    return SWITCH_PRESSED == m_currentState;
}

}  // namespace shstrailer
