#pragma once

namespace shstrailer {

class HeartBeatObserver {
   public:
    virtual ~HeartBeatObserver() = default;

    virtual void onHeartBeat() {}
};

}  // namespace shstrailer
