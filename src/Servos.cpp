#include "Servos.h"

#include <Arduino.h>
#include <Servo.h>

#include "Config.h"

namespace
{
    Servo servo1;
    Servo servo2;
    Servo servo3;

    float clampAngle(float angle)
    {
        if (angle < SERVO_MIN_DEG)
        {
            return SERVO_MIN_DEG;
        }

        if (angle > SERVO_MAX_DEG)
        {
            return SERVO_MAX_DEG;
        }

        return angle;
    }

    float applyCalibration(
        float requestedAngle,
        float center,
        float direction)
    {
        float calibrated =
            center + direction * requestedAngle;

        return clampAngle(calibrated);
    }
}

void initializeServos()
{
    servo1.attach(SERVO_1_PIN);
    servo2.attach(SERVO_2_PIN);
    servo3.attach(SERVO_3_PIN);

    centerServos();
}

void setServoAngles(const ServoCommand& command)
{
    float angle1 = applyCalibration(
        command.servo1_deg,
        SERVO_1_CENTER_DEG,
        SERVO_1_DIRECTION
    );

    float angle2 = applyCalibration(
        command.servo2_deg,
        SERVO_2_CENTER_DEG,
        SERVO_2_DIRECTION
    );

    float angle3 = applyCalibration(
        command.servo3_deg,
        SERVO_3_CENTER_DEG,
        SERVO_3_DIRECTION
    );

    servo1.write(static_cast<int>(angle1));
    servo2.write(static_cast<int>(angle2));
    servo3.write(static_cast<int>(angle3));
}

void centerServos()
{
    ServoCommand command{};
    command.servo1_deg = 0.0f;
    command.servo2_deg = 0.0f;
    command.servo3_deg = 0.0f;

    setServoAngles(command);
}

void detachServos()
{
    servo1.detach();
    servo2.detach();
    servo3.detach();
}
