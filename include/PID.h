#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "Touchscreen.h"
#include "Servos.h"

/*
 * Desired platform orientation.
 *
 * pitch_deg: rotation about the platform X axis
 * roll_deg:  rotation about the platform Y axis
 *
 * The exact sign convention must be verified during hardware testing.
 */
struct PlatformTilt
{
    float pitch_deg;
    float roll_deg;
};

/*
 * Initialize the two independent PID controllers.
 */
void initializePID();

/*
 * Calculate the desired platform pitch and roll from the current
 * ball position.
 *
 * X and Y are controlled independently using two PID controllers.
 */
PlatformTilt updatePID(const BallPosition& ball);

/*
 * Convert desired platform pitch/roll into the three actuator
 * commands for the 3-arm platform.
 *
 * NOTE:
 * This is intentionally a LINEARIZED starting model. The correct
 * inverse kinematics for your actual mechanism must be entered once
 * its geometry has been measured.
 */
ServoCommand platformTiltToServoAngles(
    const PlatformTilt& tilt
);

/*
 * Reset both PID controllers and their accumulated state.
 */
void resetPID();

#endif
