#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <Arduino.h>

/*
 * A position in millimeters relative to the center of the plate.
 *
 * x > 0 : +X direction
 * x < 0 : -X direction
 * y > 0 : +Y direction
 * y < 0 : -Y direction
 */
struct BallPosition
{
    float x_mm;
    float y_mm;
    bool valid;
};

/*
 * Initialize the physical 4-wire resistive touchscreen.
 */
void initializeTouchscreen();

/*
 * Read, calibrate, filter, and return the ball position.
 *
 * The returned coordinates are centered around (0,0) and measured
 * in millimeters using the dimensions in Config.h.
 */
BallPosition getBallPosition();

/*
 * Optional diagnostic helper for the initial hardware bring-up.
 * Prints raw X/Y/Z readings to Serial.
 */
void printRawTouchscreenData();

#endif
