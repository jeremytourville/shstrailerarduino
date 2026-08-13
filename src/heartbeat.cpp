#include "heartbeat.h"

#include "abort.h"
#include "observers/heartbeat_observer.h"

namespace shstrailer {

HeartBeat::HeartBeat(const Timer::Duration duration) : duration_(duration) {}

void HeartBeat::registerObserver(HeartBeatObserver* observer) {
    if (nullptr == observer) {
        Abort(F("heartbeat observer nullptr"));
    }

    observers_.push_back(observer);
}

void HeartBeat::update() {
    if (timer_.elapsed() > duration_) {
        timer_.start();
        notify();
    }
}

void HeartBeat::notify() {
    for (auto* observer : observers_) {
        observer->onHeartBeat();
    }
}

}  // namespace shstrailer
