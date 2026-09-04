# BallBalanceRobot

Initial software architecture for a 3-arm ball balancing robot using:
- Arduino Uno R3
- 4-wire resistive touchscreen
- 3x MG996R servos
- USB connection to a development laptop

## Architecture

src/main.cpp        Main application/control flow
src/Touchscreen.cpp Touchscreen hardware implementation
src/Servos.cpp      Servo hardware implementation
src/PID.cpp         PID controllers and platform control
include/*.h         Public interfaces/configuration

## Important

Before running the balancing controller, fill in all `USER INPUT` values in
`include/Config.h`.

Do not assume the placeholder PID gains or servo geometry are correct.
The three-servo kinematic mapping is intentionally a simple initial
linearized mapping and must eventually be replaced with the actual inverse
kinematics for the physical mechanism.

## Bring-up order

1. Compile/upload.
2. Determine touchscreen pin assignment and raw X/Y directions.
3. Calibrate touchscreen coordinates.
4. Test each servo independently.
5. Determine servo centers/directions/limits.
6. Verify the three-servo platform mapping without the ball.
7. Implement/verify actual 3-arm inverse kinematics.
8. Tune PID.
