#include "abort.h"

#include "console.h"
#include "pins.h"

namespace shstrailer {

void Abort(const __FlashStringHelper* const message) {
    pinMode(STATUS_LED, OUTPUT);

    cout << endl << F("FATAL: ") << message << endl;

    // Enter an infinite loop to halt execution. Solid status light is used to
    // indicate a fatal error has occurred.
    while (true) {
        digitalWrite(STATUS_LED, HIGH);
        delay(1);
    }
}

}  // namespace shstrailer
