#include "abort.h"

namespace shstrailer {

void Abort(const __FlashStringHelper* const message) {
    pinMode(LED_BUILTIN, OUTPUT);

    // Disable interrupts to prevent further execution
    noInterrupts();

    // Optionally, you can add a message to indicate the reason for the abort
    Serial.print(F("FATAL: "));
    Serial.println(message);

    // Enter an infinite loop to halt execution
    while (true) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);

        digitalWrite(LED_BUILTIN, LOW);
        delay(250);
    }
}

}  // namespace shstrailer
