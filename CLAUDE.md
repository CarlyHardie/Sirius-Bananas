# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

Sirius Bananas holds coursework firmware for the USAFA Astro 331 "KestrelSAT" CubeSat trainer. The root `README.md` is just a title ("Lab 8"); all current code is in `Astro331_Lab7_Code/`, a PlatformIO project (Arduino framework, ESP32-S2, board `sparkfun_esp32s2_thing_plus_c`, env `esp32thing_plus`). Run PlatformIO commands from that directory.

## Commands

```
cd Astro331_Lab7_Code
pio run                      # build
pio run -t upload            # flash the board
pio device monitor -b 115200 # USB serial console
```

There is no automated test suite. `test/` is the empty PlatformIO placeholder, and the tests under `lib/INA238-master/` belong to a vendored third-party library. "Testing" means flashing the board and running the on-device test commands (below). No linter is configured. The `pio` CLI isn't installed in the cloud container, so builds can't be verified there.

## Architecture

Firmware is a single-threaded Arduino `setup()`/`loop()` program in `src/main.cpp`, driven by a **command menu**:

- `loop()` polls every 10 ms for a command and sends a heartbeat (`*` on XBee, a prompt on USB). It does nothing else.
- `process_main_menu()` reads an integer command from **either** USB Serial (115200 baud) or the XBee UART (9600 baud, UART2) via `get_command_from_ground_station()`, then a `switch` dispatches to handlers. Menu text is duplicated for both the XBee and Serial outputs, so a menu change must be made in both places (and in the header comment table at the top of the file).
- Each experiment (`lab6_run_test`, `lab7_run_test_A`, `lab7_run_test_B`) is a **blocking loop** that runs for its whole duration, sampling every 50 ms and logging CSV rows to the SD card. Nothing else runs while a test is in progress. Reads of the command channel inside tests are blocking.
- Commands: 0 stop wheel, 1 menu, 2 RSSI, 3 LED, 4 battery, 5 manual wheel throttle, 6 Lab 6 test, 7 Lab 7A, 8 Lab 7B, 9 stream wheel speed, 98/99 SD list/print (USB only).
- Test B's prescribed wheel speed profile lives in `set_speed_test_B()`.

Supporting files:
- `include/definitions.h`: all pin assignments and constants (SD chip select, SPI, XBee, sun sensors, motor PWM, encoder, `CPR`, `MOTOR_VOLTAGE`). Change hardware wiring here. It has no include guard and is included by both `.cpp` files.
- `src/sd_funcitons.cpp` (the filename is misspelled; keep it unless renaming deliberately) implements the declarations in `include/sd_functions.h`. It owns the `SdFat` object privately. `sd_createDataFile(preamble)` auto-numbers files as `<preamble>_dataNNN.csv`, with the preamble truncated to 12 characters for FAT compatibility.
- `lib/INA238-master/`: a vendored INA238 current monitor library that is not used by `main.cpp`. Treat it as third-party code.

Hardware: ICM-20948 IMU (I2C), MAX17048 fuel gauge (I2C 0x36), TB9051FTG motor driver plus quadrature encoder (reaction wheel), 4-channel analog sun sensor, XBee radio, microSD over SPI, onboard WS2812 RGB LED (status colors: red startup, green idle, orange Lab 6, cyan Lab 7A, magenta Lab 7B).

## Gotchas

- `setup()` blocks until the USB serial monitor is opened (`while (!Serial)`), hangs forever if the IMU is not found, and loops forever printing errors if the SD card fails to initialize. A board with no SD card or IMU will appear dead.
- `sd_init()` ignores its `csPin` argument and uses the `SD_CS_PIN` macro directly.
- Library dependencies are declared in `platformio.ini` `lib_deps`; the doc comment at the top of `main.cpp` lists them too and should stay consistent.
