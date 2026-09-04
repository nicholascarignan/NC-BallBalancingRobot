#include "PID.h"

#include <Arduino.h>
#include <PID_v1.h>

#include <math.h>

#include "Config.h"

namespace
{
    double inputX = 0.0;
    double outputX = 0.0;
    double setpointX = TARGET_X_MM;

    double inputY = 0.0;
    double outputY = 0.0;
    double setpointY = TARGET_Y_MM;

    PID pidX(
        &inputX,
        &outputX,
        &setpointX,
        PID_X_KP,
        PID_X_KI,
        PID_X_KD,
        PID_X_DIRECTION
    );

    PID pidY(
        &inputY,
        &outputY,
        &setpointY,
        PID_Y_KP,
        PID_Y_KI,
        PID_Y_KD,
        PID_Y_DIRECTION
    );

    float degToRad(float degrees)
    {
        return degrees * PI / 180.0f;
    }

    float clampTilt(float value)
    {
        if (value > MAX_PLATFORM_TILT_DEG)
        {
            return MAX_PLATFORM_TILT_DEG;
        }

        if (value < -MAX_PLATFORM_TILT_DEG)
        {
            return -MAX_PLATFORM_TILT_DEG;
        }

        return value;
    }
}

void initializePID()
{
    pidX.SetOutputLimits(PID_OUTPUT_MIN, PID_OUTPUT_MAX);
    pidY.SetOutputLimits(PID_OUTPUT_MIN, PID_OUTPUT_MAX);

    /*
     * The PID library handles its own sample timing.
     * The outer robot loop is currently 50 Hz.
     */
    pidX.SetSampleTime(
        static_cast<int>(CONTROL_PERIOD_US / 1000UL)
    );

    pidY.SetSampleTime(
        static_cast<int>(CONTROL_PERIOD_US / 1000UL)
    );

    pidX.SetMode(AUTOMATIC);
    pidY.SetMode(AUTOMATIC);

    resetPID();
}

PlatformTilt updatePID(const BallPosition& ball)
{
    PlatformTilt result{};

    if (!ball.valid)
    {
        return result;
    }

    inputX = ball.x_mm;
    inputY = ball.y_mm;

    pidX.Compute();
    pidY.Compute();

    result.pitch_deg = clampTilt(static_cast<float>(outputX));
    result.roll_deg = clampTilt(static_cast<float>(outputY));

    return result;
}

ServoCommand platformTiltToServoAngles(
    const PlatformTilt& tilt)
{
    ServoCommand command{};

    /*
     * INITIAL LINEARIZED 3-SERVO MODEL
     *
     * For actuator i at azimuth theta:
     *
     *   actuator contribution ≈
     *       pitch * cos(theta)
     *     + roll  * sin(theta)
     *
     * This is NOT the final inverse kinematics.
     *
     * Replace this section with the actual geometric IK after you
     * measure the platform radius, servo-arm length, linkage length,
     * horn orientation, joint locations, and servo zero positions.
     */

    const float theta1 = degToRad(SERVO_1_AZIMUTH_DEG);
    const float theta2 = degToRad(SERVO_2_AZIMUTH_DEG);
    const float theta3 = degToRad(SERVO_3_AZIMUTH_DEG);

    command.servo1_deg =
        SERVO_TILT_GAIN_DEG *
        (tilt.pitch_deg * cos(theta1) +
         tilt.roll_deg  * sin(theta1));

    command.servo2_deg =
        SERVO_TILT_GAIN_DEG *
        (tilt.pitch_deg * cos(theta2) +
         tilt.roll_deg  * sin(theta2));

    command.servo3_deg =
        SERVO_TILT_GAIN_DEG *
        (tilt.pitch_deg * cos(theta3) +
         tilt.roll_deg  * sin(theta3));

    return command;
}

void resetPID()
{
    /*
     * The public PID library does not expose an explicit "clear
     * integral" operation. Reinitializing the controllers by switching
     * to manual and back to automatic resets their internal state.
     */
    pidX.SetMode(MANUAL);
    pidY.SetMode(MANUAL);

    outputX = 0.0;
    outputY = 0.0;

    pidX.SetMode(AUTOMATIC);
    pidY.SetMode(AUTOMATIC);
}
