#pragma once

#include "timer.h"
#include "vector.h"

namespace shstrailer {

class HeartBeatObserver;

class HeartBeat {
   public:
    explicit HeartBeat(Timer::Stamp duration = 1000);

    void update();

    void registerObserver(HeartBeatObserver* observer);

   private:
    void notify();

    Timer timer_;
    Timer::Stamp duration_;
    Vector<HeartBeatObserver*, 2> observers_;
};

}  // namespace shstrailer
