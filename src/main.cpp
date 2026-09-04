#include <Arduino.h>

#include "Config.h"
#include "Touchscreen.h"
#include "Servos.h"
#include "PID.h"

static unsigned long lastControlTime = 0;
static unsigned long lastTelemetryTime = 0;

void setup()
{
    Serial.begin(115200);

    initializeTouchscreen();
    initializeServos();
    initializePID();

    lastControlTime = micros();
    lastTelemetryTime = millis();

    Serial.println("BallBalanceRobot initialized.");
}

void loop()
{
    unsigned long nowUs = micros();

    if ((unsigned long)(nowUs - lastControlTime) < CONTROL_PERIOD_US)
        return;

    lastControlTime += CONTROL_PERIOD_US;

    BallPosition ball = getBallPosition();

    PlatformOrientation orientation =
        calculatePlatformOrientation(ball);

    ServoAngles servoAngles =
        calculateServoAngles(orientation);

    setServoAngles(servoAngles);

    // Low-rate telemetry so Serial does not interfere with the
    // high-rate control loop.
    unsigned long nowMs = millis();

    if ((unsigned long)(nowMs - lastTelemetryTime) >= TELEMETRY_PERIOD_MS)
    {
        lastTelemetryTime = nowMs;

        Serial.print("Ball X: ");
        Serial.print(ball.x_mm);
        Serial.print(" mm, Y: ");
        Serial.print(ball.y_mm);
        Serial.print(" mm | Pitch: ");
        Serial.print(orientation.pitch_deg);
        Serial.print(" deg, Roll: ");
        Serial.print(orientation.roll_deg);
        Serial.print(" deg | Servos: ");
        Serial.print(servoAngles.servo1);
        Serial.print(", ");
        Serial.print(servoAngles.servo2);
        Serial.print(", ");
        Serial.println(servoAngles.servo3);
    }
}
