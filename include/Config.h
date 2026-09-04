#pragma once

// ============================================================
// USER INPUT: HARDWARE CONFIGURATION
// ============================================================

// ---- 4-wire resistive touchscreen pins ----
// Replace these with the actual Arduino pins you use.
constexpr int TOUCH_XP = A0;   // USER INPUT
constexpr int TOUCH_XM = A1;   // USER INPUT
constexpr int TOUCH_YP = A2;   // USER INPUT
constexpr int TOUCH_YM = A3;   // USER INPUT

// Touchscreen raw calibration.
// Determine these experimentally.
// USER INPUT
constexpr int TOUCH_MIN_X = 100;
constexpr int TOUCH_MAX_X = 900;
constexpr int TOUCH_MIN_Y = 100;
constexpr int TOUCH_MAX_Y = 900;

// Reverse an axis if its physical direction is opposite to
// the coordinate convention used by the controller.
// USER INPUT: change to true if necessary.
constexpr bool TOUCH_REVERSE_X = false;
constexpr bool TOUCH_REVERSE_Y = false;

// ---- Servo pins ----
// USER INPUT: change if your wiring uses different pins.
constexpr int SERVO_1_PIN = 9;
constexpr int SERVO_2_PIN = 10;
constexpr int SERVO_3_PIN = 11;

// Servo neutral positions.
// These are the physical angles that make the platform level.
// USER INPUT: measure and adjust each servo independently.
constexpr float SERVO_1_CENTER = 90.0f;
constexpr float SERVO_2_CENTER = 90.0f;
constexpr float SERVO_3_CENTER = 90.0f;

// Servo direction.
// USER INPUT: use +1 or -1 for each servo.
constexpr float SERVO_1_DIRECTION = 1.0f;
constexpr float SERVO_2_DIRECTION = 1.0f;
constexpr float SERVO_3_DIRECTION = 1.0f;

// Safety limits.
// USER INPUT: determine safe mechanical limits for your robot.
constexpr float SERVO_MIN_ANGLE = 20.0f;
constexpr float SERVO_MAX_ANGLE = 160.0f;

// ---- Platform geometry ----
// USER INPUT.
// These are placeholders and should eventually be replaced by
// the actual geometry used by the inverse-kinematics model.
constexpr float PLATFORM_RADIUS_MM = 75.0f;
constexpr float SERVO_ARM_LENGTH_MM = 30.0f;
constexpr float SERVO_LINK_LENGTH_MM = 60.0f;

// Servo angular locations around the platform.
// USER INPUT: measure/derive from your physical geometry.
constexpr float SERVO_1_AZIMUTH_DEG = 0.0f;
constexpr float SERVO_2_AZIMUTH_DEG = 120.0f;
constexpr float SERVO_3_AZIMUTH_DEG = 240.0f;

// ---- Control ----
// Target ball position is the center of the calibrated workspace.
// USER INPUT if you want a different target.
constexpr float TARGET_X_MM = 0.0f;
constexpr float TARGET_Y_MM = 0.0f;

// PID gains.
// USER INPUT: these are deliberately placeholders.
// Start with conservative values and tune experimentally.
constexpr double PID_X_KP = 0.0;
constexpr double PID_X_KI = 0.0;
constexpr double PID_X_KD = 0.0;

constexpr double PID_Y_KP = 0.0;
constexpr double PID_Y_KI = 0.0;
constexpr double PID_Y_KD = 0.0;

// Maximum requested platform tilt.
// USER INPUT: keep this conservative during initial testing.
constexpr float MAX_PLATFORM_TILT_DEG = 8.0f;

// Controller update period.
// 10 ms = 100 Hz.
constexpr unsigned long CONTROL_PERIOD_US = 10000;

// Serial telemetry rate.
constexpr unsigned long TELEMETRY_PERIOD_MS = 100;
