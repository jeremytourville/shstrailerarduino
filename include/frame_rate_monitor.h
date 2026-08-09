#pragma once

#include <stdint.h>

#include "timer.h"

namespace shstrailer {

class FrameRateMonitor {
   public:
    void update();

    [[nodiscard]] float framesPerSecond() const;

   private:
    Timer timer_;
    uint32_t count_ = 0;
};

}  // namespace shstrailer
