#include "abort.h"

#include "console.h"

namespace shstrailer {

void Abort(const __FlashStringHelper* const message) {
    pinMode(LED_BUILTIN, OUTPUT);

    cout << endl << F("FATAL: ") << message << endl;

    // Enter an infinite loop to halt execution. Solid status light is used to
    // indicate a fatal error has occurred.
    while (true) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1);
    }
}

}  // namespace shstrailer
