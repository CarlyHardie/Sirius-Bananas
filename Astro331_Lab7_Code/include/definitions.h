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
#define SUN_MIN_SIGNAL          200.0  // CHECK: if the 4 sun sensor readings add up to less than this, "no sun"
#define TRACK_KP                0.005  // wheel command (0 to 1) per degree of sun angle error
#define TRACK_KD                0.01   // wheel command (0 to 1) per deg/s of spin rate
#define TRACK_KI                0.0    // wheel command (0 to 1) per (degree x second) of accumulated error. 0.0 = integral OFF (same as the PD controller)
#define TRACK_I_LIMIT           0.2    // the integral term can never push the wheel command more than this (0 to 1)
#define TRACK_I_ZONE_DEG        90.0   // the integral only builds up when the sun error is smaller than this (degrees)
#define TRACK_SIGN              1.0    // CHECK: change to -1.0 if the satellite turns AWAY from the sun
#define TRACK_GYRO_SIGN         -1.0   // set to -1.0 from SunTrack_data004.csv: with +1.0 the D term pushed WITH the spin instead of braking it. Flip back only if the sat oscillates
#define TRACK_MAX_TIME_MS       60000  // test stops by itself after this long
