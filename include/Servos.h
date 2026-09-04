#ifndef SERVOS_H
#define SERVOS_H

#include <Arduino.h>

struct ServoCommand
{
    float servo1_deg;
    float servo2_deg;
    float servo3_deg;
};

/*
 * Attach all three MG996R signal wires to their configured pins.
 */
void initializeServos();

/*
 * Command all three servos.
 *
 * This function applies:
 *   - servo center offsets
 *   - servo direction
 *   - safety angle limits
 */
void setServoAngles(const ServoCommand& command);

/*
 * Put all three servos at their configured neutral positions.
 */
void centerServos();

/*
 * Detach all three servos.
 * Useful for an emergency stop or mechanical setup.
 */
void detachServos();

#endif
