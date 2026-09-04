#pragma once

#include "Touchscreen.h"
#include "Servos.h"

struct PlatformOrientation
{
    float pitch_deg;
    float roll_deg;
};

void initializePID();
PlatformOrientation calculatePlatformOrientation(
    const BallPosition& ball
);

// Temporary/initial three-servo mapping.
// Replace with actual inverse kinematics once the physical geometry
// has been measured.
ServoAngles calculateServoAngles(
    const PlatformOrientation& orientation
);
