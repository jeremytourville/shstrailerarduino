#include "debounce.h"

DebouncedButton::DebouncedButton(uint8_t pin)
    : m_pin(pin), m_currentState(HIGH), m_previousState(HIGH),
      m_pressEvent(false), m_lastDebounceTime(0) {}

void DebouncedButton::begin() {
    pinMode(m_pin, INPUT_PULLUP);
    m_currentState = digitalRead(m_pin);
    m_previousState = m_currentState;
    m_lastDebounceTime = millis();
}

void DebouncedButton::update() {
    bool raw = digitalRead(m_pin);

    if (raw != m_previousState) {
        m_lastDebounceTime = millis();
        m_previousState = raw;
    }

    if ((millis() - m_lastDebounceTime) >= BUTTON_DEBOUNCE_MS) {
        if (raw != m_currentState) {
            m_currentState = raw;
            if (m_currentState == SWITCH_PRESSED) {
                m_pressEvent = true;
            }
        }
    }
}

bool DebouncedButton::wasPressed() {
    bool e = m_pressEvent;
    m_pressEvent = false;
    return e;
}

bool DebouncedButton::isPressed() const {
    return m_currentState == SWITCH_PRESSED;
}
