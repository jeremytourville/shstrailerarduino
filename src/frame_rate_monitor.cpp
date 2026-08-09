#include "frame_rate_monitor.h"

#include "console.h"

namespace shstrailer {

void FrameRateMonitor::update() {
    if (timer_.elapsed() > 1000) {
        cout << framesPerSecond() << endl;
        timer_.start();
        count_ = 0;
    }

    ++count_;
}

float FrameRateMonitor::framesPerSecond() const {
    return count_ / (timer_.elapsed() / 1000.0f);
}

}  // namespace shstrailer
