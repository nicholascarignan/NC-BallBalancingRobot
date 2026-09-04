#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/*
 * ================================================================
 * USER CONFIGURATION
 * ================================================================
 *
 * This is the main file you should edit while bringing up the robot.
 * Values marked "USER INPUT" MUST be replaced with measurements from
 * your actual hardware.
 */

// ----------------------------------------------------------------
// SERIAL / CONTROL LOOP
// ----------------------------------------------------------------
constexpr unsigned long SERIAL_BAUD = 115200;

// Start at 50 Hz. Increase only after the sensor/servo system works.
constexpr unsigned long CONTROL_PERIOD_US = 20000UL;

// Send debug telemetry at a much slower rate than the control loop.
constexpr unsigned long TELEMETRY_PERIOD_MS = 100;

// ----------------------------------------------------------------
// SERVO PINS
// ----------------------------------------------------------------
// USER INPUT: Change these if your servo signal wires use different pins.
constexpr uint8_t SERVO_1_PIN = 9;
constexpr uint8_t SERVO_2_PIN = 10;
constexpr uint8_t SERVO_3_PIN = 11;

// ----------------------------------------------------------------
// TOUCHSCREEN PINS
// ----------------------------------------------------------------
// These are for a RAW 4-wire resistive panel using Adafruit TouchScreen.
//
// Constructor order is:
// TouchScreen(XP, YP, XM, YM, resistance)
//
// USER INPUT: Set these to match the labels/wires on YOUR panel.
//
// Analog pins A0-A3 can be used as digital I/O on the Uno.
constexpr uint8_t TOUCH_XP_PIN = A0;
constexpr uint8_t TOUCH_YP_PIN = A1;
constexpr uint8_t TOUCH_XM_PIN = A2;
constexpr uint8_t TOUCH_YM_PIN = A3;

// USER INPUT:
// Approximate resistance of the panel's X plate in ohms.
// 300 is a common starting point; the exact value is not critical
// for basic position readings.
constexpr int TOUCH_RX_PLATE = 300;

// ----------------------------------------------------------------
// TOUCHSCREEN RAW CALIBRATION
// ----------------------------------------------------------------
// First run the touchscreen test program / telemetry and record the
// raw X/Y values at the physical edges of the usable plate.
//
// USER INPUT: Replace these four values with measured raw readings.
//
// IMPORTANT: If the raw axes move in the opposite direction from the
// physical axes, set the corresponding INVERT value to true.
//
// Example:
//   Touch raw X increases when the ball physically moves left:
//       TOUCH_INVERT_X = true
//
constexpr int TOUCH_RAW_X_MIN = 100;   // USER INPUT
constexpr int TOUCH_RAW_X_MAX = 900;   // USER INPUT
constexpr int TOUCH_RAW_Y_MIN = 100;   // USER INPUT
constexpr int TOUCH_RAW_Y_MAX = 900;   // USER INPUT

constexpr bool TOUCH_INVERT_X = false; // USER INPUT
constexpr bool TOUCH_INVERT_Y = false; // USER INPUT

// ----------------------------------------------------------------
// PHYSICAL PLATE SIZE
// ----------------------------------------------------------------
// USER INPUT: Measure the actual usable ball-contact area in mm.
//
// The coordinate system used by this project is centered at (0, 0).
constexpr float PLATE_WIDTH_MM  = 200.0f; // USER INPUT
constexpr float PLATE_HEIGHT_MM = 200.0f; // USER INPUT

// ----------------------------------------------------------------
// TOUCH FILTERING
// ----------------------------------------------------------------
// A simple exponential moving average is used initially.
//
// 0.0 = completely frozen
// 1.0 = no filtering
//
// USER INPUT: Start around 0.25-0.40 and tune experimentally.
constexpr float TOUCH_FILTER_ALPHA = 0.30f;

// Minimum pressure reported by the touchscreen that is considered valid.
// USER INPUT: Adjust if your raw Z values require it.
constexpr int TOUCH_MIN_PRESSURE = 10;

// ----------------------------------------------------------------
// SERVO CALIBRATION
// ----------------------------------------------------------------
// These are the physical neutral positions of your three servos.
//
// USER INPUT: Put the platform mechanically level and determine the
// command angle that makes each servo/linkage sit at its true neutral.
// Do NOT assume all three are exactly 90 degrees.
//
// The Servo library accepts approximately 0-180 degrees.
constexpr float SERVO_1_CENTER_DEG = 90.0f; // USER INPUT
constexpr float SERVO_2_CENTER_DEG = 90.0f; // USER INPUT
constexpr float SERVO_3_CENTER_DEG = 90.0f; // USER INPUT

// Direction of each servo's positive command.
//
// USER INPUT:
// Set to +1 if increasing the commanded angle produces positive
// mathematical actuator motion.
// Set to -1 if it produces the opposite motion.
constexpr float SERVO_1_DIRECTION = 1.0f; // USER INPUT
constexpr float SERVO_2_DIRECTION = 1.0f; // USER INPUT
constexpr float SERVO_3_DIRECTION = 1.0f; // USER INPUT

// Safety limits. Start conservatively.
//
// USER INPUT: Determine safe mechanical limits before balancing.
constexpr float SERVO_MIN_DEG = 20.0f;
constexpr float SERVO_MAX_DEG = 160.0f;

// ----------------------------------------------------------------
// PLATFORM / SERVO GEOMETRY
// ----------------------------------------------------------------
// IMPORTANT:
// The exact inverse kinematics of your 3-arm mechanism depends on
// your physical geometry. These values are deliberately placeholders.
//
// The initial software uses a LINEARIZED 3-servo tilt mapping:
// each servo receives a combination of pitch and roll.
//
// Once the mechanism dimensions are measured, replace the mapping
// in PID.cpp with the actual inverse kinematics.
//
// Servo arm angular locations around the platform (degrees).
// USER INPUT: Measure/derive the three actuator directions.
constexpr float SERVO_1_AZIMUTH_DEG = 0.0f;   // USER INPUT
constexpr float SERVO_2_AZIMUTH_DEG = 120.0f; // USER INPUT
constexpr float SERVO_3_AZIMUTH_DEG = 240.0f; // USER INPUT

// Servo motion per degree of requested platform tilt.
// USER INPUT / CALIBRATION:
// These are NOT universal MG996R values. They are a placeholder
// linearization gain that must be experimentally determined.
constexpr float SERVO_TILT_GAIN_DEG = 1.0f; // USER INPUT

// ----------------------------------------------------------------
// CONTROL TARGET
// ----------------------------------------------------------------
// Desired ball position. Usually the physical center.
constexpr float TARGET_X_MM = 0.0f;
constexpr float TARGET_Y_MM = 0.0f;

// ----------------------------------------------------------------
// PID GAINS
// ----------------------------------------------------------------
// USER INPUT:
// These are starting values only. They are NOT expected to balance
// the robot without tuning.
//
// X and Y are controlled independently.
//
// Start with:
//   Kp > 0
//   Ki = 0
//   Kd = 0
//
// Tune P first, then D, and add I only if there is persistent offset.
constexpr double PID_X_KP = 0.0; // USER INPUT
constexpr double PID_X_KI = 0.0; // USER INPUT
constexpr double PID_X_KD = 0.0; // USER INPUT

constexpr double PID_Y_KP = 0.0; // USER INPUT
constexpr double PID_Y_KI = 0.0; // USER INPUT
constexpr double PID_Y_KD = 0.0; // USER INPUT

// Maximum requested platform tilt.
// USER INPUT: Start small, e.g. 3-5 degrees.
constexpr double MAX_PLATFORM_TILT_DEG = 5.0;

// PID library output limits.
constexpr double PID_OUTPUT_MIN = -MAX_PLATFORM_TILT_DEG;
constexpr double PID_OUTPUT_MAX =  MAX_PLATFORM_TILT_DEG;

// ----------------------------------------------------------------
// PID DIRECTION
// ----------------------------------------------------------------
// USER INPUT / CRITICAL:
//
// The sign depends on your coordinate system and the mechanical
// direction of the platform.
//
// Start with DIRECT. If the controller consistently drives the ball
// farther away from the center, reverse the appropriate axis.
constexpr int PID_X_DIRECTION = 0; // 0 = DIRECT, 1 = REVERSE
constexpr int PID_Y_DIRECTION = 0; // 0 = DIRECT, 1 = REVERSE

#endif
