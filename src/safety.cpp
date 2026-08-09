#include "safety.h"

namespace shstrailer {

SafetyController::SafetyController() : m_ledState(false) {
    m_status.systemState = SystemState::STARTUP;
    m_status.battery = {0.0f, false, false};
    m_status.winchFault = false;
    m_status.winchCooldown = false;
}

void SafetyController::begin() {
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, LOW);

    m_ledState = false;
    m_ledTimer.start();
    m_batteryTimer.start();
}

void SafetyController::safeStartup() {
    // Explicitly force every controlled output OFF before entering READY.
    digitalWrite(LIGHT1_OUT, OUTPUT_OFF);
    digitalWrite(LIGHT2_OUT, OUTPUT_OFF);
    digitalWrite(LIGHT3_OUT, OUTPUT_OFF);
    digitalWrite(LIGHT4_OUT, OUTPUT_OFF);
    digitalWrite(LED_STRIP_OUT, OUTPUT_OFF);
    digitalWrite(POD_LIGHT_OUT, OUTPUT_OFF);
    digitalWrite(WINCH_UP_OUT, OUTPUT_OFF);
    digitalWrite(WINCH_DN_OUT, OUTPUT_OFF);

    m_status.systemState = SystemState::STARTUP;

    delay(STARTUP_LOCKOUT_MS);

    if (ENABLE_BATTERY_MONITOR) {
        readBattery();
    }

    updateSystemState();
}

void SafetyController::readBattery() {
    uint32_t adcTotal = 0;

    for (uint8_t i = 0; i < BATTERY_AVERAGE_SAMPLES; ++i) {
        adcTotal += analogRead(BATTERY_VOLTAGE_PIN);
    }

    const float adcAverage = (float)adcTotal / (float)BATTERY_AVERAGE_SAMPLES;

    const float vPin =
        (adcAverage * ADC_REFERENCE_VOLTAGE) / (float)ADC_MAX_COUNTS;

    const float dividerRatio =
        (VOLTAGE_DIVIDER_R1 + VOLTAGE_DIVIDER_R2) / VOLTAGE_DIVIDER_R2;

    const float vBatt = vPin * dividerRatio;

    m_status.battery.voltage = vBatt;
    m_status.battery.warning = (vBatt <= BATTERY_WARNING_VOLT);
    m_status.battery.critical = (vBatt <= BATTERY_CRITICAL_VOLT);
}

void SafetyController::setWinchFault(bool faulted) {
    m_status.winchFault = faulted;
}

void SafetyController::setWinchCooldown(bool coolingDown) {
    m_status.winchCooldown = coolingDown;
}

void SafetyController::updateSystemState() {
    if (m_status.winchFault || m_status.winchCooldown) {
        m_status.systemState = SystemState::FAULT;
    } else if (ENABLE_BATTERY_MONITOR && m_status.battery.critical) {
        m_status.systemState = SystemState::LOW_BATTERY;
    } else {
        m_status.systemState = SystemState::READY;
    }
}

void SafetyController::updateStatusLED() {
    if (!ENABLE_STATUS_LED) {
        digitalWrite(STATUS_LED, LOW);
        return;
    }

    switch (m_status.systemState) {
        case SystemState::STARTUP:
            // LED off during startup lockout.
            digitalWrite(STATUS_LED, LOW);
            m_ledState = false;
            break;

        case SystemState::READY:
            // Slow heartbeat: short visual activity without looking like an
            // alarm.
            if (m_ledTimer.elapsed() >= 1000UL) {
                m_ledTimer.start();
                m_ledState = !m_ledState;
                digitalWrite(STATUS_LED, m_ledState ? HIGH : LOW);
            }
            break;

        case SystemState::LOW_BATTERY:
            // Faster blink for critical battery voltage.
            if (m_ledTimer.elapsed() >= 250UL) {
                m_ledTimer.start();
                m_ledState = !m_ledState;
                digitalWrite(STATUS_LED, m_ledState ? HIGH : LOW);
            }
            break;

        case SystemState::FAULT:
            // Very fast blink for winch/runtime fault.
            if (m_ledTimer.elapsed() >= 100UL) {
                m_ledTimer.start();
                m_ledState = !m_ledState;
                digitalWrite(STATUS_LED, m_ledState ? HIGH : LOW);
            }
            break;
    }
}

void SafetyController::update() {
    if (ENABLE_BATTERY_MONITOR &&
        m_batteryTimer.elapsed() >= BATTERY_SAMPLE_INTERVAL_MS) {
        m_batteryTimer.start();
        readBattery();
    }

    updateSystemState();
    updateStatusLED();
}

const ControllerStatus& SafetyController::status() const { return m_status; }

}  // namespace shstrailer
