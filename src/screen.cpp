#include "screen.h"

#include <Adafruit_GFX.h>
#include <Wire.h>

#include "abort.h"

namespace shstrailer {

Screen::Screen() : display_(128, 64, &Wire, -1) {}

void Screen::drawText(const int16_t x, const int16_t y, const char* text) {
    initialize();

    display_.setTextSize(1);
    display_.setTextColor(SSD1309_PIXEL_ON);
    display_.setCursor(x, y);
    display_.println(text);
    display_.display();
}

void Screen::initialize() {
    if (initialized_) {
        return;
    }

    // screen address is 0x3C
    if (!display_.begin(SSD1309_SWITCHCAPVCC, 0x3C)) {
        Abort(F("Failed to initialize screen"));
    }

    initialized_ = true;
}

}  // namespace shstrailer
