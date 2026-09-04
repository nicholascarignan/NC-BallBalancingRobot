#pragma once

struct ServoAngles
{
    float servo1;
    float servo2;
    float servo3;
};

void initializeServos();
void setServoAngles(const ServoAngles& angles);
void setServo1(float angle);
void setServo2(float angle);
void setServo3(float angle);
