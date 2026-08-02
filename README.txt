Trailer Controller - Revised Firmware

Major revisions

1. Winch safety
- If UP and DOWN buttons are pressed together, the command becomes STOP.
- Winch outputs are never intentionally allowed on at the same time.
- Direction changes force both outputs OFF for WINCH_DIRECTION_DELAY_MS.
- The 30-second runtime limit now enters a true FAULT state.
- A runtime fault requires:
    a) both buttons released, and
    b) WINCH_FAULT_COOLDOWN_MS to expire.
- Winch state and fault status can be queried by other modules.

2. Battery monitor
- Battery voltage is no longer sampled every loop pass.
- Sampling occurs every BATTERY_SAMPLE_INTERVAL_MS.
- Each reported reading is averaged over BATTERY_AVERAGE_SAMPLES ADC readings.
- Existing 100k / 33k divider and warning/critical thresholds were retained.

3. System status
- SystemState is now actively used.
- READY = normal.
- LOW_BATTERY = critical battery threshold reached.
- FAULT = winch runtime fault.
- Status LED:
    STARTUP: off
    READY: 1-second toggle heartbeat
    LOW_BATTERY: 250 ms toggle
    FAULT: 100 ms toggle

4. Lighting
- Added range checking before indexing the light array.
- Added isOn() helper for future diagnostics.
- Startup explicitly resets internal light states to OFF.

5. Debouncing
- Logic is functionally the same, with clearer initialization and use of one
  millis() value per update.

Important hardware note
Software interlocking is useful, but a winch reversing contactor should ideally
also have a hardware/electrical interlock so both direction coils cannot be
energized simultaneously due to a wiring fault, failed MOSFET, or controller
fault.


5% winch duty-cycle revision
- Maximum continuous operation is now 45 seconds.
- A true 5% duty cycle requires 19 seconds OFF for every second ON.
- A full 45-second run therefore requires 855 seconds OFF: 14 minutes 15 seconds.
- Short runs receive proportional cooldown.
- The winch is inhibited during cooldown and releasing the button does not bypass it.
- A 45-second timeout remains faulted until the buttons are released and cooldown expires.
