#ifndef WINCH_H
#define WINCH_H
#include <Arduino.h>
#include "config.h"
#include "pins.h"
#include "types.h"

class WinchController {
public:
    WinchController();
    void begin();
    void update();
    void commandUp();
    void commandDown();
    void stop();

private:
    WinchState m_state;
    WinchDirection m_requested;
    uint32_t m_timer;
    void setOutputs(bool up, bool down);
};

#endif
