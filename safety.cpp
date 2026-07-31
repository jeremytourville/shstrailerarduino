#include "safety.h"

SafetyController::SafetyController()
    : m_ledTimer(0), m_ledState(false) {
    m_status.systemState = SystemState::STARTUP;
    m_status.battery = {0.0f, false, false};
}

void SafetyController::begin() {
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, LOW);
}

void SafetyController::safeStartup() {
    digitalWrite(LIGHT1_OUT, OUTPUT_OFF);
    digitalWrite(LIGHT2_OUT, OUTPUT_OFF);
    digitalWrite(LIGHT3_OUT, OUTPUT_OFF);
    digitalWrite(LIGHT4_OUT, OUTPUT_OFF);
    digitalWrite(LED_STRIP_OUT, OUTPUT_OFF);
    digitalWrite(POD_LIGHT_OUT, OUTPUT_OFF);
    digitalWrite(WINCH_UP_OUT, OUTPUT_OFF);
    digitalWrite(WINCH_DN_OUT, OUTPUT_OFF);

    delay(STARTUP_LOCKOUT_MS);

    m_status.systemState = SystemState::READY;
}

void SafetyController::readBattery() {
    uint16_t adc = analogRead(BATTERY_VOLTAGE_PIN);
    float vPin = (adc * ADC_REFERENCE_VOLTAGE) / ADC_MAX_COUNTS;
    float vBatt = vPin * ((VOLTAGE_DIVIDER_R1 + VOLTAGE_DIVIDER_R2) / VOLTAGE_DIVIDER_R2);

    m_status.battery.voltage = vBatt;
    m_status.battery.warning = (vBatt <= BATTERY_WARNING_VOLT);
    m_status.battery.critical = (vBatt <= BATTERY_CRITICAL_VOLT);
}

void SafetyController::updateStatusLED() {
    if (!ENABLE_STATUS_LED) return;

    uint32_t now = millis();
    uint16_t interval = m_status.battery.critical ? 250 : 1000;

    if ((now - m_ledTimer) >= interval) {
        m_ledTimer = now;
        m_ledState = !m_ledState;
        digitalWrite(STATUS_LED, m_ledState ? HIGH : LOW);
    }
}

void SafetyController::update() {
    if (ENABLE_BATTERY_MONITOR) {
        readBattery();
    }
    updateStatusLED();
}
