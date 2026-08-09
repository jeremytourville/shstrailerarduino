#pragma once

#include "button_observer.hpp"
#include "vector.h"

namespace shstrailer {

class Light;

class LightController : public ButtonObserver {
   public:
    void registerLight(Light* light);

    void onLongPressed() override;

    void off();

   private:
    Vector<Light*> lights_;
};

}  // namespace shstrailer
