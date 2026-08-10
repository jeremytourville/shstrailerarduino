#pragma once

#include <DIYables_OLED_SSD1309.h>
#include <stdint.h>

namespace shstrailer {

class Screen {
   public:
    Screen();

    void drawText(int16_t x, int16_t y, const char* text);

   private:
    void initialize();

    DIYables_OLED_SSD1309 display_;
    bool initialized_ = false;
};

}  // namespace shstrailer
