# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## About Us
-We are 3rd year engineering students who are new to codeing, Arduino, C++, and using codeing agents. Code using simple logic over clever code
-please explain everything in simple terms

## Hardware - SparkFun Thing Plus ESP32​
PlatformIO env: esp32thing_plus​
XBee: HardwareSerial(2), 9600 baud​
ICM-20948 IMU, MAX17048 gauge (I2C)​
Pin map: include/definitions.h ​

## Conventions - Commands: process_main_menu()​
Telemetry: Name:value,Name:value\n​
Mirror every XBee message to Serial ​

## Rules - Never change pins without asking​
Add libraries only via lib_deps​
No blocking waits in loop()​
You can't test on hardware: list​
what I must verify on KesteralSat
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
- Commands: 0 stop wheel, 1 menu, 2 RSSI, 3 LED, 4 battery, 5 manual wheel throttle, 6 Lab 6 test, 7 Lab 7A, 8 Lab 7B, 9 stream wheel speed, 10 sun tracking test (closed loop, `sun_tracking_test()`), 98/99 SD list/print (USB only).
- Test B's prescribed wheel speed profile lives in `set_speed_test_B()`.
- Sun tracking (command 10) is a PID controller. The sun position comes from opposite-face differences in raw counts (`sun_X` = +X minus -X, `sun_Y` = +Y minus -Y, so room light that reaches both faces cancels); `sun_direction` is `atan2(sun_Y, sun_X)` and "sun visible" means the four faces add up to at least `SUN_MIN_SIGNAL`. Error = `sun_direction` minus `SUN_TARGET_ANGLE_DEG` (180, wrapped to ±180); P from `TRACK_KP`, D from `TRACK_KD` and gyro Z, I from `TRACK_KI` (0 = off, i.e. PD; set to 0.0005 after a hardware log showed a steady 24 deg offset) with `TRACK_I_LIMIT` and `TRACK_I_ZONE_DEG` against windup. The sun sensor faces only report which face sees the light (`sun_direction` is 0/90/180/270), so once the -X face sees it (`TRACK_CENTER_ON_PEAK` = 1) the code switches modes: SCAN (turn gently while the -X reading rises), then HOLD (after it falls below `TRACK_PEAK_DROP` of the best value, turn back by the gyro-measured angle and hold; this only counts if the best -X reading is at least `TRACK_PEAK_MIN_FRACTION` of the brightest face reading in the run and the satellite is turning faster than `TRACK_PEAK_MIN_RATE_DPS`, and HOLD has its own I term, `TRACK_HOLD_KI`). Scan and search need about 0.28 wheel command to turn the satellite (0.15 left it stuck in `SunTrack_data038.csv`). If no face sees the light for `TRACK_LOST_SAMPLES` readings, the satellite keeps the same wheel command it had just before the light was lost (at least `TRACK_SEARCH_PWM`, at most `TRACK_SEARCH_MAX_PWM`) in the direction it was turning, until a face sees the light, giving up after `TRACK_SEARCH_MAX_MS`. During the test the XBee (and a Serial mirror) carries a live line `sDir:<deg>,s+X:<counts>,s-X:<counts>,s+Y:<counts>,s-Y:<counts>,V:<panel volts>,I_mA:<panel mA>,P_mW:<panel mW>` 10 times a second (`TRACK_XBEE_EVERY`; the line is about 78 characters worst case, 81% of the 9600 baud XBee link, so raise `TRACK_XBEE_EVERY` if the ground station drops characters) for the ground station, instead of the old `tp:<n>` point counter. The `mode` column in the CSV shows 0 APPROACH / 1 SCAN / 2 HOLD / 3 SEARCH. Gains and signs are in `definitions.h` (the INA238 is read every sample during the test; the CSV ends with `panel_V(V)`, `panel_I(mA)` and `panel_P(mW)` = V × mA, so 22 columns, and they are 0 if the INA238 was not found); `TRACK_GYRO_SIGN` is -1.0 (found from a hardware log); the gain, `TRACK_SIGN` and `SUN_MIN_SIGNAL` values marked CHECK there are still unverified.

Supporting files:
- `include/definitions.h`: all pin assignments and constants (SD chip select, SPI, XBee, sun sensors, motor PWM, encoder, `CPR`, `MOTOR_VOLTAGE`). Change hardware wiring here. It has no include guard and is included by both `.cpp` files.
- `src/sd_funcitons.cpp` (the filename is misspelled; keep it unless renaming deliberately) implements the declarations in `include/sd_functions.h`. It owns the `SdFat` object privately. `sd_createDataFile(preamble)` auto-numbers files as `<preamble>_dataNNN.csv`, with the preamble truncated to 12 characters for FAT compatibility.
- `Simulation/SunTrackSim.html`: a standalone browser simulator of command 10: a line-by-line port of the control step in `sun_tracking_test()` (PID mode 0, scan 1, hold 2, search 3) with the `definitions.h` constants as editable parameters. It uses the firmware's gains, units and sign conventions, models the 4-face sun sensor, wheel deadband and motor lag, and exports/overlays CSVs with the SunTrack column names. Default inertias are estimates fitted to SunTrack_data004 (0.34 deg/s per wheel RPM). Keep it in sync if the controller math in `sun_tracking_test()` changes.
- `lib/INA238-master/`: a vendored INA238 current monitor library that is not used by `main.cpp`. Treat it as third-party code.

Hardware: ICM-20948 IMU (I2C), MAX17048 fuel gauge (I2C 0x36), TB9051FTG motor driver plus quadrature encoder (reaction wheel), 4-channel analog sun sensor, XBee radio, microSD over SPI, onboard WS2812 RGB LED (status colors: red startup, green idle, orange Lab 6, cyan Lab 7A, magenta Lab 7B).

## Gotchas

- `setup()` blocks until the USB serial monitor is opened (`while (!Serial)`), hangs forever if the IMU is not found, and loops forever printing errors if the SD card fails to initialize. A board with no SD card or IMU will appear dead.
- `sd_init()` ignores its `csPin` argument and uses the `SD_CS_PIN` macro directly.
- Library dependencies are declared in `platformio.ini` `lib_deps`; the doc comment at the top of `main.cpp` lists them too and should stay consistent.
