#include "Servos.h"
#include "Config.h"

#include <Arduino.h>
#include <Servo.h>

static Servo servo1;
static Servo servo2;
static Servo servo3;

static float constrainAngle(float angle)
{
    return constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
}

void initializeServos()
{
    servo1.attach(SERVO_1_PIN);
    servo2.attach(SERVO_2_PIN);
    servo3.attach(SERVO_3_PIN);

    setServo1(SERVO_1_CENTER);
    setServo2(SERVO_2_CENTER);
    setServo3(SERVO_3_CENTER);
}

void setServo1(float angle)
{
    angle = SERVO_1_CENTER +
            (angle - SERVO_1_CENTER) * SERVO_1_DIRECTION;

    servo1.write(static_cast<int>(constrainAngle(angle)));
}

void setServo2(float angle)
{
    angle = SERVO_2_CENTER +
            (angle - SERVO_2_CENTER) * SERVO_2_DIRECTION;

    servo2.write(static_cast<int>(constrainAngle(angle)));
}

void setServo3(float angle)
{
    angle = SERVO_3_CENTER +
            (angle - SERVO_3_CENTER) * SERVO_3_DIRECTION;

    servo3.write(static_cast<int>(constrainAngle(angle)));
}

void setServoAngles(const ServoAngles& angles)
{
    setServo1(angles.servo1);
    setServo2(angles.servo2);
    setServo3(angles.servo3);
}
