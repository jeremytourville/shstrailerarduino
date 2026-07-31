#include "winch.h"

WinchController::WinchController()
    : m_state(WinchState::IDLE),
      m_requested(WinchDirection::STOP),
      m_timer(0) {}

void WinchController::begin() {
    pinMode(WINCH_UP_OUT, OUTPUT);
    pinMode(WINCH_DN_OUT, OUTPUT);
    setOutputs(false, false);
}

void WinchController::commandUp() { m_requested = WinchDirection::UP; }
void WinchController::commandDown() { m_requested = WinchDirection::DOWN; }
void WinchController::stop() { m_requested = WinchDirection::STOP; }

void WinchController::setOutputs(bool up, bool down) {
    digitalWrite(WINCH_UP_OUT, up ? OUTPUT_ON : OUTPUT_OFF);
    digitalWrite(WINCH_DN_OUT, down ? OUTPUT_ON : OUTPUT_OFF);
}

void WinchController::update() {
    switch (m_state) {
        case WinchState::IDLE:
            if (m_requested == WinchDirection::UP) {
                setOutputs(true, false);
                m_state = WinchState::RUNNING_UP;
                m_timer = millis();
            } else if (m_requested == WinchDirection::DOWN) {
                setOutputs(false, true);
                m_state = WinchState::RUNNING_DOWN;
                m_timer = millis();
            }
            break;

        case WinchState::RUNNING_UP:
            if (m_requested == WinchDirection::STOP) {
                setOutputs(false, false);
                m_state = WinchState::IDLE;
            } else if (m_requested == WinchDirection::DOWN) {
                setOutputs(false, false);
                m_state = WinchState::DIRECTION_DELAY;
                m_timer = millis();
            } else if ((millis() - m_timer) > WINCH_MAX_RUNTIME_MS) {
                setOutputs(false, false);
                m_state = WinchState::FAULT;
            }
            break;

        case WinchState::RUNNING_DOWN:
            if (m_requested == WinchDirection::STOP) {
                setOutputs(false, false);
                m_state = WinchState::IDLE;
            } else if (m_requested == WinchDirection::UP) {
                setOutputs(false, false);
                m_state = WinchState::DIRECTION_DELAY;
                m_timer = millis();
            } else if ((millis() - m_timer) > WINCH_MAX_RUNTIME_MS) {
                setOutputs(false, false);
                m_state = WinchState::FAULT;
            }
            break;

        case WinchState::DIRECTION_DELAY:
            if ((millis() - m_timer) >= WINCH_DIRECTION_DELAY) {
                m_state = WinchState::IDLE;
            }
            break;

        case WinchState::FAULT:
            setOutputs(false, false);
            if (m_requested == WinchDirection::STOP) {
                m_state = WinchState::IDLE;
            }
            break;
    }
}
