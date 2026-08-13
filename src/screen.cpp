#include "screen.h"

#include <Adafruit_GFX.h>

#include "algorithm.h"
#include "console.h"

namespace shstrailer {

namespace {

const uint8_t kHeartBitmap[] PROGMEM = {
    0x6C,  // 01101100 -> . X X . X X . (Row 1)
    0xFE,  // 11111110 -> X X X X X X X (Row 2)
    0xFE,  // 11111110 -> X X X X X X X (Row 3)
    0x7C,  // 01111100 -> . X X X X X . (Row 4)
    0x38,  // 00111000 -> . . X X X . . (Row 5)
    0x10,  // 00010000 -> . . . X . . . (Row 6)
    0x00   // 00000000 -> . . . . . . . (Row 7 - Bottom Spacing)
};

constexpr Timer::Stamp kFrameDuration = 250;

}  // namespace

Screen::Screen() : display_(128, 64) {
    snprintf(versionString_, sizeof(versionString_), "%s", GetVersionString());
}

void Screen::onBatteryVoltage(const float voltage) {
    batteryVoltage_ = voltage;
}

void Screen::onBatteryState([[maybe_unused]] const BatteryState state) {
    batteryState_ = state;
}

void Screen::onWinchState(const WinchState state,
                          const Timer::Stamp cooldownTimeRemaining) {
    winchState_ = state;
    winchCooldownTimeRemaining_ = cooldownTimeRemaining;
}

void Screen::onHeartBeat() { drawHeartbeat_ = !drawHeartbeat_; }

void Screen::beginDisplay() {
    if (!initialize()) {
        return;
    }

    display_.clearDisplay();
}

void Screen::endDisplay() {
    if (!initialize()) {
        return;
    }

    // Do not draw every frame, saves power.
    if (timer_.elapsed() < kFrameDuration) {
        return;
    }

    timer_.start();

    drawBatteryGroup();

    drawWinchGroup();

    drawStatusBar();

    display_.display();
}

void Screen::drawBattery(const int16_t x, const int16_t y) {
    if (BatteryState::CRITICAL == batteryState_ && !drawHeartbeat_) {
        return;
    }

    //  body
    display_.drawRect(x + 2, y, 36, 10, SSD1309_PIXEL_ON);

    // cap
    display_.fillRect(x, y + 3, 2, 4, SSD1309_PIXEL_ON);

    constexpr int16_t kCellWidth = 10;
    constexpr int16_t kCellHeight = 6;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"

    // intentional fall through
    switch (batteryState_) {
        case BatteryState::OK:
            // cell 1
            display_.fillRect(x + 4, y + 2, kCellWidth, kCellHeight,
                              SSD1309_PIXEL_ON);
        case BatteryState::WARNING:
            // cell 2
            display_.fillRect(x + 5 + kCellWidth, y + 2, kCellWidth,
                              kCellHeight, SSD1309_PIXEL_ON);
        case BatteryState::CRITICAL:
            // cell 3
            display_.fillRect(x + 6 + 2 * kCellWidth, y + 2, kCellWidth,
                              kCellHeight, SSD1309_PIXEL_ON);

            break;
    }

#pragma GCC diagnostic pop
}

void Screen::drawBatteryGroup() {
    display_.drawRoundRect(0, 0, 128, 26, 3, SSD1309_PIXEL_ON);
    display_.fillRoundRect(6, 0, 45, 9, 2, SSD1309_PIXEL_ON);

    drawText(8, 1, "BATTERY", 1, SSD1309_PIXEL_OFF);

    constexpr size_t kBufferSize = kVoltageBufferSize + 1;
    char buffer[kBufferSize];

    if (snprintf(buffer, sizeof(buffer), "%sV", voltageToString()) > 0) {
        drawText(4, 10, buffer, 2);
    }

    drawBattery(87, 2);
}

void Screen::drawWinchGroup() {
    display_.drawRoundRect(0, 27, 128, 27, 3, SSD1309_PIXEL_ON);
    display_.fillRoundRect(6, 27, 33, 9, 2, SSD1309_PIXEL_ON);

    drawText(8, 28, "WINCH", 1, SSD1309_PIXEL_OFF);

    if (winchCooldownTimeRemaining_ > 0) {
        drawCoolDownTimeRemaining();
        return;
    }

    switch (winchState_) {
        case WinchState::IDLE:
            drawText(15, 37, "READY", 2);
            break;

        case WinchState::FAULT:
            drawText(15, 37, "FAULT", 2);
            break;

        case WinchState::RUNNING_UP:
            drawText(15, 37, "UP", 2);
            break;

        case WinchState::RUNNING_DOWN:
            drawText(15, 37, "DOWN", 2);
            break;

        case WinchState::DIRECTION_DELAY:
            // intentionally blank
            break;
    }
}

void Screen::drawCoolDownTimeRemaining() {
    // convert to seconds, minutes
    const auto totalSeconds = winchCooldownTimeRemaining_ / 1000;
    const auto seconds = totalSeconds % 60;
    const auto minutes = minimum(99UL, totalSeconds / 60);

    constexpr size_t kBufferSize = 7;
    char buffer[kBufferSize];

    if (snprintf(buffer, kBufferSize, "%02lu:%02lu", minutes, seconds) > 0) {
        drawText(15, 37, buffer, 2);
    }
}

void Screen::drawStatusBar() {
    display_.drawFastHLine(0, bottomHLineY_ + 1, 128, SSD1309_PIXEL_ON);

    if (drawHeartbeat_) {
        display_.drawBitmap(3, 57, kHeartBitmap, 7, 7, SSD1309_PIXEL_ON);
    }

    drawText(versionX_, versionY_ + 1, versionString_);
}

void Screen::drawText(const int16_t x, const int16_t y, const char* text,
                      const uint8_t textSize, const uint16_t color) {
    if (!initialize()) {
        return;
    }

    display_.setTextColor(color);
    display_.setTextSize(textSize);
    display_.setCursor(x, y);
    display_.print(text);
}

bool Screen::initialize() {
    if (initialized_) {
        return initialized_ && available_;
    }

    if (display_.begin()) {
        available_ = true;
        timer_.start();

        int16_t junk = 0;
        uint16_t width;
        uint16_t height;
        display_.getTextBounds(versionString_, junk, junk, &junk, &junk, &width,
                               &height);

        versionX_ = display_.width() - width;
        versionY_ = display_.height() - height;
        bottomHLineY_ = versionY_ - 2;
    } else {
        available_ = false;
        cout << F("screen begin failed") << endl;
    }

    initialized_ = true;

    return initialized_ && available_;
}

const char* Screen::voltageToString() {
    constexpr float kVoltageRange = 99.99f;

    // Keep voltage in a range so the buffer boundary isnt violated.
    return dtostrf(clamp(batteryVoltage_, -kVoltageRange, kVoltageRange),
                   kVoltageBufferSize - 1, 2, voltageString_);
}

}  // namespace shstrailer
