#pragma once

#include <stdint.h>

namespace shstrailer {

class LightObserver {
   public:
    virtual ~LightObserver() = default;

    virtual void onLightState([[maybe_unused]] uint8_t pin,
                              [[maybe_unused]] uint8_t state) {}
};

}  // namespace shstrailer
