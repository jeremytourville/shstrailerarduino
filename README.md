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


### Testing on cirkitdesigner.com

In order to get the screen to work go to the "Code" tab, then on the left select "Custom Libraries" and upload the DIYables_OLED_SSD1309-1.0.2.zip file.

The zip file is the source code from the DIYables github found here:
https://github.com/DIYables/DIYables_OLED_SSD1309/releases/tag/v1.0.2

# Wiring
The screen has 4 pins:
VDD
GND
SCK
SDA

VDD goes to the 5V pin on the arduino board.
GND goes to the GND pin on the arduino board.
SCK goes to the SCL21 pin on the arduino board.
SDA goes to the SDA20 pin on the arduino board.
