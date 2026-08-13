#include "battery.h"

#include "abort.h"
#include "pins.h"

namespace shstrailer {

namespace {

constexpr Timer::Stamp kSampleInterval = 250;
constexpr Timer::Stamp kStateDuration = 1000UL * 60UL * 30UL;  // 30 minutes
constexpr uint8_t kAverageSamples = 8;
constexpr float kADCReferenceVoltage = 5.0f;
constexpr uint16_t kADCMaxCounts = 1023;
constexpr float kVoltageDividerR1 = 100000.0f;
constexpr float kVoltageDividerR2 = 33000.0f;
constexpr float kWarningVoltage = 12.20f;
constexpr float kCriticalVoltage = 11.80f;

}  // namespace

void Battery::update() {
    if (sampleTimer_.elapsed() < kSampleInterval) {
        return;
    }

    sampleTimer_.start();

    readVoltage();

    notifyVoltage();

    if (voltage_ < kCriticalVoltage) {
        if (BatteryState::CRITICAL != potentialState_) {
            stateTimer_.start();
            potentialState_ = BatteryState::CRITICAL;
        }
    } else if (voltage_ < kWarningVoltage) {
        if (BatteryState::WARNING != potentialState_) {
            stateTimer_.start();
            potentialState_ = BatteryState::WARNING;
        }
    } else if (BatteryState::OK != potentialState_) {
        stateTimer_.start();
        potentialState_ = BatteryState::OK;
    }

    // The battery has been in a new potential state long enough
    // to set it as that state.
    if (state_ != potentialState_ && stateTimer_.elapsed() > kStateDuration) {
        state_ = potentialState_;
        notifyState();
    }
}

void Battery::registerObserver(BatteryObserver* observer) {
    if (nullptr == observer) {
        Abort(F("battery observer nullptr"));
    }

    observers_.push_back(observer);

    // provide initial state
    observer->onBatteryState(state_);
}

void Battery::notifyVoltage() {
    for (auto* observer : observers_) {
        observer->onBatteryVoltage(voltage_);
    }
}

void Battery::notifyState() {
    for (auto* observer : observers_) {
        observer->onBatteryState(state_);
    }
}

void Battery::readVoltage() {
    uint32_t adcTotal = 0;

    for (uint8_t i = 0; i < kAverageSamples; ++i) {
        adcTotal += analogRead(BATTERY_VOLTAGE_PIN);
    }

    const float adcAverage = (float)adcTotal / (float)kAverageSamples;

    const float vPin =
        (adcAverage * kADCReferenceVoltage) / (float)kADCMaxCounts;

    const float dividerRatio =
        (kVoltageDividerR1 + kVoltageDividerR2) / kVoltageDividerR2;

    voltage_ = vPin * dividerRatio;
}

}  // namespace shstrailer
