#include "PID.h"
#include "Config.h"

#include <Arduino.h>
#include <PID_v1.h>
#include <math.h>

static double inputX = 0.0;
static double outputX = 0.0;
static double setpointX = TARGET_X_MM;

static double inputY = 0.0;
static double outputY = 0.0;
static double setpointY = TARGET_Y_MM;

static PID pidX(
    &inputX,
    &outputX,
    &setpointX,
    PID_X_KP,
    PID_X_KI,
    PID_X_KD,
    DIRECT
);

static PID pidY(
    &inputY,
    &outputY,
    &setpointY,
    PID_Y_KP,
    PID_Y_KI,
    PID_Y_KD,
    DIRECT
);

void initializePID()
{
    pidX.SetOutputLimits(
        -MAX_PLATFORM_TILT_DEG,
        MAX_PLATFORM_TILT_DEG
    );

    pidY.SetOutputLimits(
        -MAX_PLATFORM_TILT_DEG,
        MAX_PLATFORM_TILT_DEG
    );

    pidX.SetMode(AUTOMATIC);
    pidY.SetMode(AUTOMATIC);
}

PlatformOrientation calculatePlatformOrientation(
    const BallPosition& ball
)
{
    PlatformOrientation orientation{0.0f, 0.0f};

    if (!ball.valid)
        return orientation;

    inputX = ball.x_mm;
    inputY = ball.y_mm;

    pidX.Compute();
    pidY.Compute();

    // NOTE:
    // The sign convention must be experimentally verified.
    // If the platform moves the ball in the wrong direction,
    // either reverse the PID direction or the corresponding axis.
    orientation.pitch_deg = static_cast<float>(outputX);
    orientation.roll_deg  = static_cast<float>(outputY);

    return orientation;
}

ServoAngles calculateServoAngles(
    const PlatformOrientation& orientation
)
{
    ServoAngles result{
        SERVO_1_CENTER,
        SERVO_2_CENTER,
        SERVO_3_CENTER
    };

    // --------------------------------------------------------
    // INITIAL LINEARIZED 3-SERVO MAPPING
    // --------------------------------------------------------
    //
    // This is NOT the final inverse kinematics.
    // It exists so the software architecture can be tested.
    //
    // For a proper 3-arm platform, these equations need to be
    // derived from the actual arm/link/platform geometry.
    //
    const float pitch = orientation.pitch_deg;
    const float roll  = orientation.roll_deg;

    constexpr float SQRT3_OVER_2 = 0.8660254f;

    float command1 = roll;
    float command2 =
        -0.5f * roll + SQRT3_OVER_2 * pitch;
    float command3 =
        -0.5f * roll - SQRT3_OVER_2 * pitch;

    result.servo1 = SERVO_1_CENTER + command1;
    result.servo2 = SERVO_2_CENTER + command2;
    result.servo3 = SERVO_3_CENTER + command3;

    return result;
}
