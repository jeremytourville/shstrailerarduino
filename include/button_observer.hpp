#pragma once

namespace shstrailer {

class ButtonObserver {
   public:
    virtual ~ButtonObserver() = default;

    virtual void onPressed() {};

    virtual void onLongPressed() {};
};

}  // namespace shstrailer
