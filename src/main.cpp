#include <Arduino.h>

#include "Config.h"
#include "PID.h"
#include "Servos.h"
#include "Touchscreen.h"

/*
 * ================================================================
 * BALL BALANCING ROBOT - MAIN
 * ================================================================
 *
 * Hardware:
 *   - Arduino Uno R3
 *   - 3x MG996R hobby servos
 *   - 4-wire resistive touchscreen
 *   - USB connection to development laptop
 *
 * The control flow is intentionally simple:
 *
 *   touchscreen
 *       ↓
 *   ball position
 *       ↓
 *   PID
 *       ↓
 *   desired platform tilt
 *       ↓
 *   3-servo kinematics
 *       ↓
 *   servo commands
 */

namespace
{
    unsigned long lastControlTime = 0;
    unsigned long lastTelemetryTime = 0;

    BallPosition lastBall{};
    PlatformTilt lastTilt{};
    ServoCommand lastServoCommand{};
}

void setup()
{
    Serial.begin(SERIAL_BAUD);

    initializeTouchscreen();
    initializeServos();
    initializePID();

    centerServos();

    Serial.println();
    Serial.println("=================================");
    Serial.println(" Ball Balancing Robot");
    Serial.println(" Arduino Uno R3");
    Serial.println("=================================");
    Serial.println("System initialized.");
    Serial.println("WARNING: PID gains and geometry");
    Serial.println("must be configured before balancing.");
}

void loop()
{
    const unsigned long nowUs = micros();

    if (static_cast<unsigned long>(nowUs - lastControlTime)
        < CONTROL_PERIOD_US)
    {
        return;
    }

    lastControlTime = nowUs;

    /*
     * 1. Read ball position from the touchscreen.
     */
    lastBall = getBallPosition();

    /*
     * 2. If there is no valid touch, do not command a correction.
     *
     * During initial testing this is deliberately conservative.
     */
    if (!lastBall.valid)
    {
        centerServos();
        lastTilt = {};
        lastServoCommand = {};
        return;
    }

    /*
     * 3. PID:
     *    ball position → desired platform pitch/roll
     */
    lastTilt = updatePID(lastBall);

    /*
     * 4. Kinematics:
     *    platform pitch/roll → three servo angles
     */
    lastServoCommand =
        platformTiltToServoAngles(lastTilt);

    /*
     * 5. Command the three MG996R servos.
     */
    setServoAngles(lastServoCommand);

    /*
     * 6. Low-rate telemetry.
     *
     * NEVER put high-volume Serial printing in the control loop.
     */
    const unsigned long nowMs = millis();

    if (static_cast<unsigned long>(
            nowMs - lastTelemetryTime) >= TELEMETRY_PERIOD_MS)
    {
        lastTelemetryTime = nowMs;

        Serial.print("Ball X=");
        Serial.print(lastBall.x_mm, 2);

        Serial.print(" Y=");
        Serial.print(lastBall.y_mm, 2);

        Serial.print(" | Pitch=");
        Serial.print(lastTilt.pitch_deg, 2);

        Serial.print(" Roll=");
        Serial.print(lastTilt.roll_deg, 2);

        Serial.print(" | S1=");
        Serial.print(lastServoCommand.servo1_deg, 2);

        Serial.print(" S2=");
        Serial.print(lastServoCommand.servo2_deg, 2);

        Serial.print(" S3=");
        Serial.println(lastServoCommand.servo3_deg, 2);
    }
}
