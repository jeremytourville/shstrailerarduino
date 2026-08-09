#include "abort.h"

#include "console.h"

namespace shstrailer {

void Abort(const __FlashStringHelper* const message) {
    pinMode(LED_BUILTIN, OUTPUT);

    // Disable interrupts to prevent further execution
    noInterrupts();

    cout << endl << F("FATAL: ") << message << endl;

    // Enter an infinite loop to halt execution
    while (true) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);

        digitalWrite(LED_BUILTIN, LOW);
        delay(250);
    }
}

}  // namespace shstrailer
