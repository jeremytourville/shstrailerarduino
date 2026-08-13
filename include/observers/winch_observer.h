#pragma once

#include "timer.h"
#include "types.h"

namespace shstrailer {

class WinchObserver {
   public:
    virtual ~WinchObserver() = default;

    virtual void onWinchState(
        [[maybe_unused]] WinchState state,
        [[maybe_unused]] Timer::Stamp cooldownTimeRemaining) {}
};

}  // namespace shstrailer
