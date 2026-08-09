#include "winch.h"

namespace shstrailer {

WinchController::WinchController()
    : m_state(WinchState::IDLE),
      m_requested(WinchDirection::STOP),
      m_stateTimer(0),
      m_runStartTime(0),
      m_cooldownStartTime(0),
      m_requiredCooldownMs(0),
      m_coolingDown(false) {}

void WinchController::begin() {
    pinMode(WINCH_UP_OUT, OUTPUT);
    pinMode(WINCH_DN_OUT, OUTPUT);
    setOutputs(false, false);
    m_state = WinchState::IDLE;
    m_requested = WinchDirection::STOP;
    const uint32_t now = millis();
    m_stateTimer = now;
    m_runStartTime = now;
    m_cooldownStartTime = now;
    m_requiredCooldownMs = 0;
    m_coolingDown = false;
}

void WinchController::commandUp() { m_requested = WinchDirection::UP; }

void WinchController::commandDown() { m_requested = WinchDirection::DOWN; }

void WinchController::stop() { m_requested = WinchDirection::STOP; }

void WinchController::setOutputs(bool up, bool down) {
    if (up && down) {
        up = false;
        down = false;
    }

    digitalWrite(WINCH_UP_OUT, up ? OUTPUT_ON : OUTPUT_OFF);
    digitalWrite(WINCH_DN_OUT, down ? OUTPUT_ON : OUTPUT_OFF);
}

void WinchController::beginRun(WinchDirection direction, uint32_t now) {
    if (m_coolingDown || direction == WinchDirection::STOP) {
        setOutputs(false, false);
        return;
    }

    m_runStartTime = now;

    if (direction == WinchDirection::UP) {
        setOutputs(true, false);
        m_state = WinchState::RUNNING_UP;
    } else {
        setOutputs(false, true);
        m_state = WinchState::RUNNING_DOWN;
    }
}

void WinchController::endRunAndStartCooldown(uint32_t now) {
    setOutputs(false, false);

    const uint32_t runTimeMs = now - m_runStartTime;

    if (runTimeMs > (UINT32_MAX / WINCH_OFF_TO_ON_RATIO)) {
        m_requiredCooldownMs = UINT32_MAX;
    } else {
        m_requiredCooldownMs = runTimeMs * WINCH_OFF_TO_ON_RATIO;
    }

    m_cooldownStartTime = now;
    m_coolingDown = (m_requiredCooldownMs > 0);
}

void WinchController::enterFault(uint32_t now) {
    endRunAndStartCooldown(now);
    m_state = WinchState::FAULT;
}

void WinchController::updateCooldown(uint32_t now) {
    if (!m_coolingDown) {
        return;
    }

    if ((uint32_t)(now - m_cooldownStartTime) >= m_requiredCooldownMs) {
        m_coolingDown = false;
        m_requiredCooldownMs = 0;
    }
}

void WinchController::update() {
    const uint32_t now = millis();
    updateCooldown(now);

    switch (m_state) {
        case WinchState::IDLE:
            setOutputs(false, false);
            if (!m_coolingDown) {
                if (m_requested == WinchDirection::UP)
                    beginRun(WinchDirection::UP, now);
                else if (m_requested == WinchDirection::DOWN)
                    beginRun(WinchDirection::DOWN, now);
            }
            break;

        case WinchState::RUNNING_UP:
            if (m_requested == WinchDirection::STOP) {
                endRunAndStartCooldown(now);
                m_state = WinchState::IDLE;
            } else if (m_requested == WinchDirection::DOWN) {
                endRunAndStartCooldown(now);
                m_state = WinchState::DIRECTION_DELAY;
                m_stateTimer = now;
            } else if ((uint32_t)(now - m_runStartTime) >=
                       WINCH_MAX_CONTINUOUS_RUNTIME_MS) {
                enterFault(now);
            }
            break;

        case WinchState::RUNNING_DOWN:
            if (m_requested == WinchDirection::STOP) {
                endRunAndStartCooldown(now);
                m_state = WinchState::IDLE;
            } else if (m_requested == WinchDirection::UP) {
                endRunAndStartCooldown(now);
                m_state = WinchState::DIRECTION_DELAY;
                m_stateTimer = now;
            } else if ((uint32_t)(now - m_runStartTime) >=
                       WINCH_MAX_CONTINUOUS_RUNTIME_MS) {
                enterFault(now);
            }
            break;

        case WinchState::DIRECTION_DELAY:
            setOutputs(false, false);
            if (m_requested == WinchDirection::STOP) {
                m_state = WinchState::IDLE;
            } else if ((uint32_t)(now - m_stateTimer) >=
                       WINCH_DIRECTION_DELAY_MS) {
                m_state = WinchState::IDLE;
            }
            break;

        case WinchState::FAULT:
            setOutputs(false, false);
            if (m_requested == WinchDirection::STOP && !m_coolingDown) {
                m_state = WinchState::IDLE;
            }
            break;
    }
}

bool WinchController::isFaulted() const { return m_state == WinchState::FAULT; }

bool WinchController::isCoolingDown() const { return m_coolingDown; }

WinchState WinchController::state() const { return m_state; }

uint32_t WinchController::cooldownRemainingMs() const {
    if (!m_coolingDown) {
        return 0;
    }

    const uint32_t elapsed = millis() - m_cooldownStartTime;

    if (elapsed >= m_requiredCooldownMs) {
        return 0;
    }

    return m_requiredCooldownMs - elapsed;
}

void WinchController::onContinuousPress(const uint8_t pin) {
    // Winch is hold-to-run. Both buttons cannot be pressed because it uses a
    // momentary rocker.
    switch (pin) {
        case WINCH_UP_SW:
            commandUp();
            break;
        case WINCH_DN_SW:
            commandDown();
            break;
    }
}

void WinchController::onReleased([[maybe_unused]] const uint8_t pin) {
    // Winch is subscribed only to WINCH_UP_SW and WINCH_DN_SW, no other
    // button will cause this to be called.

    stop();
}

}  // namespace shstrailer
