/*---------------------------------------------------------------------------------------------*/
// Configuration:
/*---------------------------------------------------------------------------------------------*/
#define SD_CS_PIN 5  // Chip select pin for the microSD card on Thing Plus
// #define SD_CS_PIN 33  // Chip select pin for the microSD card on Feather Adalogger
#define SPI_SCK   18
#define SPI_MISO  19
#define SPI_MOSI  23

#define XBEE_RX 16  // ESP32 RX <- XBee DOUT
#define XBEE_TX 17  // ESP32 TX -> XBee DIN

#define SUN_SENSOR_PLUS_X_PIN   A0
#define SUN_SENSOR_MINUS_X_PIN  A2
#define SUN_SENSOR_PLUS_Y_PIN   A1
#define SUN_SENSOR_MINUS_Y_PIN  A3

#define MOTOR_VOLTAGE           5.05
#define CPR                     64
#define MOTOR_PWM_1_PIN         33
#define MOTOR_PWM_2_PIN         15
#define ENCODER_PIN_A           14
#define ENCODER_PIN_B           32

// Solar panel current/voltage monitor (INA238 on I2C):
#define INA238_SHUNT_OHMS       0.015  // same shunt value as Lab 3 (ohms)
#define INA238_MAX_CURRENT_A    0.5    // same max current as Lab 3 (amps)

// Sun tracking (command 10):
#define SUN_TARGET_ANGLE_DEG    180.0  // sun_direction value when the solar panel faces the sun
#define SUN_TOLERANCE_DEG       45.0   // goal: stay within +/- this angle of the target
#define SUN_MIN_SIGNAL          30.0   // CHECK: if the 4 sun sensor readings add up to less than this, "no sun" (dark reads exactly 0 in the logs)
#define TRACK_KP                0.005  // wheel command (0 to 1) per degree of sun angle error
#define TRACK_KD                0.01   // wheel command (0 to 1) per deg/s of spin rate
#define TRACK_KI                0.0005 // wheel command (0 to 1) per (degree x second) of accumulated error. Start here; double if the offset goes away too slowly, halve if it swings. 0.0 = integral OFF
#define TRACK_I_LIMIT           0.2    // the integral term can never push the wheel command more than this (0 to 1)
#define TRACK_I_ZONE_DEG        90.0   // the integral only builds up when the sun error is smaller than this (degrees)
#define TRACK_SIGN              1.0    // CHECK: change to -1.0 if the satellite turns AWAY from the sun
#define TRACK_GYRO_SIGN         -1.0   // set to -1.0 from SunTrack_data004.csv: with +1.0 the D term pushed WITH the spin instead of braking it. Flip back only if the sat oscillates
#define TRACK_MAX_TIME_MS       60000  // test stops by itself after this long

// Stopping at the brightest point of the -X face (command 10):
#define TRACK_CENTER_ON_PEAK    1      // 1 = once the -X face sees the light, stop where it reads brightest. 0 = old behavior (error from sun_direction only)
#define TRACK_PEAK_MIN_COUNT    100.0  // the -X face must read at least this (and be the brightest face) to count as "sees the light"
#define TRACK_IN_VIEW_SAMPLES   3      // readings in a row needed before we trust that the -X face sees the light
#define TRACK_OUT_VIEW_SAMPLES  10     // readings in a row without the -X face seeing the light before we give up and go back to the coarse controller
#define TRACK_BRIGHT_ALPHA      0.3    // smoothing of the -X reading (0 to 1; smaller = smoother but slower)
#define TRACK_PEAK_DROP         0.93   // we have passed the brightest point when the reading falls below this fraction of the best value seen
#define TRACK_SCAN_PWM          0.28   // CHECK: wheel command while scanning. SunTrack_data038: 0.15 left the satellite turning ~2 dps; about 0.28 turned it ~17 dps
#define TRACK_MIN_PWM           0.12   // CHECK: smallest push used to get a resting wheel moving while going back to the brightest point
#define TRACK_HOLD_DEADZONE_DEG 3.0    // while holding, angle errors smaller than this are ignored
#define TRACK_HOLD_KI           0.002  // CHECK: while holding, wheel command per (degree x second) of angle error added up, so the push grows if we stall short of the brightest point
#define TRACK_PEAK_MIN_FRACTION 0.30   // "passed the brightest point" only counts if the best -X reading is at least this fraction of the brightest face reading seen in the run
#define TRACK_PEAK_MIN_RATE_DPS 5.0    // ...and only while the satellite is really turning (more than this many deg/s)

// Searching when no face sees the light (command 10):
#define TRACK_SEARCH_WHEN_LOST  1      // 1 = if no face sees the light, keep turning the way we were going until one does. 0 = hold the wheel at 0
#define TRACK_LOST_SAMPLES      2      // readings in a row with no light before the search starts (2 x 50 ms = 0.1 s, so it keeps going almost without a pause)
#define TRACK_SEARCH_PWM        0.28   // CHECK: smallest wheel command used while searching (same reason as TRACK_SCAN_PWM)
#define TRACK_SEARCH_MAX_PWM    0.50   // the search never pushes harder than this, even if the last command was bigger
#define TRACK_SEARCH_MAX_MS     20000  // give up after this long without finding light (roughly one full turn, an estimate)
