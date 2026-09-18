#include "screen.h"

#include <Adafruit_GFX.h>
#include <stdio.h>

#include "algorithm.h"
#include "console.h"
#include "pins.h"

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

constexpr Timer::Duration kFrameDuration = 250;
constexpr Timer::Duration kInitRetryDuration = 1000;

}  // namespace

Screen::Screen() : display_(128, 64) {
    snprintf(versionString_, sizeof(versionString_), "%s", GetVersionString());
}

void Screen::onLightState(const uint8_t pin, const uint8_t state) {
    uint8_t lightMask = 0;

    switch (pin) {
        case LIGHT1_OUT:
            lightMask = kLight1Mask;
            break;
        case LIGHT2_OUT:
            lightMask = kLight2Mask;
            break;
        case LIGHT3_OUT:
            lightMask = kLight3Mask;
            break;
        case LIGHT4_OUT:
            lightMask = kLight4Mask;
            break;
        case POD_LIGHT_OUT:
            lightMask = kDualPodLightsMask;
            break;
        case LED_STRIP_OUT:
            lightMask = kLEDStripLightMask;
            break;
    }

    if (HIGH == state) {
        lightStates_ |= lightMask;
    } else {
        lightStates_ &= ~lightMask;
    }
}

void Screen::onWinchState(const WinchState state,
                          const Timer::Duration cooldownTimeRemaining) {
    winchState_ = state;
    winchCooldownTimeRemaining_ = cooldownTimeRemaining;
}

void Screen::onHeartBeat() { drawHeartbeat_ = !drawHeartbeat_; }

void Screen::update() {
    if (!initialize()) {
        return;
    }

    // Do not draw every frame, saves power.
    if (timer_.elapsed() < kFrameDuration) {
        return;
    }

    timer_.start();
    display_.clearDisplay();

    drawLightsGroup();

    drawWinchGroup();

    drawStatusBar();

    display_.display();
}

void Screen::drawLightsGroup() {
    display_.drawRoundRect(0, 0, 128, 26, 3, SSD1309_PIXEL_ON);
    display_.fillRoundRect(6, 0, 38, 9, 2, SSD1309_PIXEL_ON);

    drawText(8, 1, "LIGHTS", 1, SSD1309_PIXEL_OFF);

    display_.drawRect(46, 3, 80, 20, SSD1309_PIXEL_ON);

    // curb side nose door
    display_.drawRect(46 + 7, 3, 7, 3, SSD1309_PIXEL_ON);

    // curb side middle doors
    display_.drawRect(46 + 20, 3, 4, 3, SSD1309_PIXEL_ON);
    display_.drawRect(46 + 23, 3, 4, 3, SSD1309_PIXEL_ON);

    // curb side tail doors
    display_.drawRect(46 + 54, 3, 4, 3, SSD1309_PIXEL_ON);
    display_.drawRect(46 + 57, 3, 4, 3, SSD1309_PIXEL_ON);

    // rear doors
    display_.drawRect(46 + 80 - 3, 3, 3, 10, SSD1309_PIXEL_ON);
    display_.drawRect(46 + 80 - 3, 13, 3, 10, SSD1309_PIXEL_ON);

    // street side doors
    display_.drawRect(46 + 35, 20, 7, 3, SSD1309_PIXEL_ON);
    display_.drawRect(46 + 42, 20, 7, 3, SSD1309_PIXEL_ON);

    // light 1
    if (lightStates_ & kLight1Mask) {
        display_.fillCircle(46 + 10, 9, 2, SSD1309_PIXEL_ON);
    } else {
        display_.drawCircle(46 + 10, 9, 2, SSD1309_PIXEL_ON);
    }

    // light 2
    if (lightStates_ & kLight2Mask) {
        display_.fillCircle(46 + 23, 9, 2, SSD1309_PIXEL_ON);
    } else {
        display_.drawCircle(46 + 23, 9, 2, SSD1309_PIXEL_ON);
    }

    // light 3
    if (lightStates_ & kLight3Mask) {
        display_.fillCircle(46 + 57, 9, 2, SSD1309_PIXEL_ON);
    } else {
        display_.drawCircle(46 + 57, 9, 2, SSD1309_PIXEL_ON);
    }

    // light 4
    if (lightStates_ & kLight4Mask) {
        display_.fillCircle(46 + 30, 18, 2, SSD1309_PIXEL_ON);
    } else {
        display_.drawCircle(46 + 30, 18, 2, SSD1309_PIXEL_ON);
    }

    // dual pod lights
    if (lightStates_ & kDualPodLightsMask) {
        display_.fillCircle(46 + 73, 7, 2, SSD1309_PIXEL_ON);
        display_.fillCircle(46 + 73, 18, 2, SSD1309_PIXEL_ON);

    } else {
        display_.drawCircle(46 + 73, 7, 2, SSD1309_PIXEL_ON);
        display_.drawCircle(46 + 73, 18, 2, SSD1309_PIXEL_ON);
    }

    // led strip light on
    display_.drawFastHLine(50, 13, 70, SSD1309_PIXEL_ON);

    if (0 == (lightStates_ & kLEDStripLightMask)) {
        // led strip light off dashed
        for (int i = 0; i < 70; i += 6) {
            display_.drawFastHLine(50 + i, 13, 2, SSD1309_PIXEL_OFF);
        }
    }
}

void Screen::drawWinchGroup() {
    display_.drawRoundRect(0, 27, 128, 27, 3, SSD1309_PIXEL_ON);
    display_.fillRoundRect(6, 27, 33, 9, 2, SSD1309_PIXEL_ON);

    drawText(8, 28, "WINCH", 1, SSD1309_PIXEL_OFF);

    switch (winchState_) {
        case WinchState::IDLE:
            drawText(15, 37, "READY", 2);
            break;

        case WinchState::RUNNING_UP:
            drawText(15, 37, "UP", 2);
            break;

        case WinchState::RUNNING_DOWN:
            drawText(15, 37, "DOWN", 2);
            break;

        case WinchState::COOLING_DOWN:
            drawCoolDownTimeRemaining();
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
    display_.drawFastHLine(0, bottomHLineY_, 128, SSD1309_PIXEL_ON);

    if (drawHeartbeat_) {
        display_.drawBitmap(3, 57, kHeartBitmap, 7, 7, SSD1309_PIXEL_ON);
    }

    drawText(versionX_, versionY_, versionString_);
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
        return true;
    }

    // keep trying to connect to the screen
    if (timer_.elapsed() < kInitRetryDuration) {
        return false;
    }

    timer_.start();

    if (display_.begin()) {
        timer_.start();

        int16_t junk = 0;
        uint16_t width;
        uint16_t height;
        display_.getTextBounds(versionString_, junk, junk, &junk, &junk, &width,
                               &height);

        versionX_ = display_.width() - width;
        versionY_ = (display_.height() - height) + 1;
        bottomHLineY_ = versionY_ - 2;
        initialized_ = true;
    }

    return initialized_;
}

}  // namespace shstrailer
