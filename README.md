# Trailer Controller

Complete PlatformIO firmware project for the Arduino Mega 2560 based trailer lighting and winch controller.

Features:
- Safe startup (all outputs OFF)
- Four two-location lighting circuits
- Two single-location lighting circuits
- Non-blocking software debounce
- Winch UP/DOWN interlock
- Battery voltage monitoring
- Status LED

Build:
    pio run

Upload:
    pio run --target upload

Serial monitor:
    pio device monitor
