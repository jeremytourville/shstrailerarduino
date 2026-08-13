#pragma once

namespace shstrailer {
class Screen;

class ScreenSentinel {
   public:
    ScreenSentinel(Screen& screen);

    ScreenSentinel(const ScreenSentinel&) = delete;
    ScreenSentinel(ScreenSentinel&&) = delete;
    ScreenSentinel& operator=(const ScreenSentinel&) = delete;
    ScreenSentinel& operator=(ScreenSentinel&&) = delete;

    ~ScreenSentinel();

   private:
    Screen& screen_;
};

}  // namespace shstrailer
