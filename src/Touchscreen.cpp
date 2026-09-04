#include "Touchscreen.h"

#include <Arduino.h>
#include <TouchScreen.h>

#include "Config.h"

namespace
{
    TouchScreen touchscreen(
        TOUCH_XP_PIN,
        TOUCH_YP_PIN,
        TOUCH_XM_PIN,
        TOUCH_YM_PIN,
        TOUCH_RX_PLATE
    );

    float filteredX = 0.0f;
    float filteredY = 0.0f;
    bool filterInitialized = false;

    float mapRawToPhysical(
        int raw,
        int rawMin,
        int rawMax,
        float physicalMin,
        float physicalMax,
        bool invert)
    {
        if (rawMax == rawMin)
        {
            return 0.0f;
        }

        float normalized =
            (static_cast<float>(raw) - rawMin) /
            static_cast<float>(rawMax - rawMin);

        if (invert)
        {
            normalized = 1.0f - normalized;
        }

        return physicalMin +
               normalized * (physicalMax - physicalMin);
    }
}

void initializeTouchscreen()
{
    filteredX = 0.0f;
    filteredY = 0.0f;
    filterInitialized = false;
}

BallPosition getBallPosition()
{
    TSPoint point = touchscreen.getPoint();

    BallPosition result{};
    result.valid = false;

    if (point.z < TOUCH_MIN_PRESSURE)
    {
        return result;
    }

    float x_mm = mapRawToPhysical(
        point.x,
        TOUCH_RAW_X_MIN,
        TOUCH_RAW_X_MAX,
        -PLATE_WIDTH_MM / 2.0f,
        +PLATE_WIDTH_MM / 2.0f,
        TOUCH_INVERT_X
    );

    float y_mm = mapRawToPhysical(
        point.y,
        TOUCH_RAW_Y_MIN,
        TOUCH_RAW_Y_MAX,
        -PLATE_HEIGHT_MM / 2.0f,
        +PLATE_HEIGHT_MM / 2.0f,
        TOUCH_INVERT_Y
    );

    if (!filterInitialized)
    {
        filteredX = x_mm;
        filteredY = y_mm;
        filterInitialized = true;
    }
    else
    {
        filteredX =
            TOUCH_FILTER_ALPHA * x_mm +
            (1.0f - TOUCH_FILTER_ALPHA) * filteredX;

        filteredY =
            TOUCH_FILTER_ALPHA * y_mm +
            (1.0f - TOUCH_FILTER_ALPHA) * filteredY;
    }

    result.x_mm = filteredX;
    result.y_mm = filteredY;
    result.valid = true;

    return result;
}

void printRawTouchscreenData()
{
    TSPoint point = touchscreen.getPoint();

    Serial.print("Raw X: ");
    Serial.print(point.x);

    Serial.print(" | Raw Y: ");
    Serial.print(point.y);

    Serial.print(" | Raw Z: ");
    Serial.println(point.z);
}
