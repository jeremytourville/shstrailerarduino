#include "screen_sentinel.h"

#include "screen.h"

namespace shstrailer {

ScreenSentinel::ScreenSentinel(Screen& screen) : screen_(screen) {
    screen_.beginDisplay();
}

ScreenSentinel::~ScreenSentinel() { screen_.endDisplay(); }

}  // namespace shstrailer
