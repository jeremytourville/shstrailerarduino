#include "debounce.h"

DebouncedButton::DebouncedButton(uint8_t pin)
    : m_pin(pin),
      m_currentState(SWITCH_RELEASED),
      m_previousState(SWITCH_RELEASED),
      m_pressEvent(false),
      m_lastDebounceTime(0) {}

void DebouncedButton::begin() {
    pinMode(m_pin, INPUT_PULLUP);

    const bool initialState = digitalRead(m_pin);
    m_currentState = initialState;
    m_previousState = initialState;
    m_pressEvent = false;
    m_lastDebounceTime = millis();
}

void DebouncedButton::update() {
    const bool raw = digitalRead(m_pin);
    const uint32_t now = millis();

    if (raw != m_previousState) {
        m_previousState = raw;
        m_lastDebounceTime = now;
    }

    if ((uint32_t)(now - m_lastDebounceTime) >= BUTTON_DEBOUNCE_MS) {
        if (raw != m_currentState) {
            m_currentState = raw;

            if (m_currentState == SWITCH_PRESSED) {
                m_pressEvent = true;
            }
        }
    }
}

bool DebouncedButton::wasPressed() {
    const bool event = m_pressEvent;
    m_pressEvent = false;
    return event;
}

bool DebouncedButton::isPressed() const {
    return m_currentState == SWITCH_PRESSED;
}
