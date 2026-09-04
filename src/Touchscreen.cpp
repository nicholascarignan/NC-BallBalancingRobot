#include "Touchscreen.h"
#include "Config.h"

#include <Arduino.h>
#include <Adafruit_TouchScreen.h>

// Adafruit TouchScreen constructor uses XP, YP, XM, YM.
static Adafruit_TouchScreen touchscreen(
    TOUCH_XP, TOUCH_YP, TOUCH_XM, TOUCH_YM, 300
);

void initializeTouchscreen()
{
    // The Adafruit library configures the pins dynamically while reading.
}

static float mapFloat(
    float value,
    float inMin,
    float inMax,
    float outMin,
    float outMax
)
{
    return (value - inMin) * (outMax - outMin) /
           (inMax - inMin) + outMin;
}

BallPosition getBallPosition()
{
    BallPosition result{0.0f, 0.0f, false};

    TSPoint p = touchscreen.getPoint();

    // Adafruit TouchScreen returns pressure Z. The exact threshold
    // depends on your hardware and should be experimentally adjusted.
    if (p.z <= 0)
        return result;

    float x = mapFloat(
        static_cast<float>(p.x),
        TOUCH_MIN_X,
        TOUCH_MAX_X,
        -1.0f,
        1.0f
    );

    float y = mapFloat(
        static_cast<float>(p.y),
        TOUCH_MIN_Y,
        TOUCH_MAX_Y,
        -1.0f,
        1.0f
    );

    if (TOUCH_REVERSE_X)
        x = -x;

    if (TOUCH_REVERSE_Y)
        y = -y;

    // Convert normalized coordinates to millimeters.
    // This uses PLATFORM_RADIUS as a temporary workspace scale.
    result.x_mm = x * PLATFORM_RADIUS_MM;
    result.y_mm = y * PLATFORM_RADIUS_MM;
    result.valid = true;

    return result;
}
