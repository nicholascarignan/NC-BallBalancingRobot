# BallBalanceRobot

Software architecture for a 3-arm ball-balancing robot using:

- Arduino Uno R3
- 3 × MG996R hobby servos
- 4-wire resistive touchscreen
- USB connection to a development laptop

The project is written as an Arduino-framework C++ project using
[PlatformIO](https://platformio.org/), so the main application is
`src/main.cpp` rather than an Arduino `.ino` sketch.

## Architecture

```text
src/main.cpp
    |
    +---- Touchscreen.h/.cpp
    |          |
    |          +---- Adafruit TouchScreen library
    |
    +---- PID.h/.cpp
    |          |
    |          +---- Brett Beauregard PID library
    |          |
    |          +---- 3-servo platform mapping / future inverse kinematics
    |
    +---- Servos.h/.cpp
    |          |
    |          +---- Arduino Servo library
    |
    +---- Config.h
               |
               +---- pins
               +---- touchscreen calibration
               +---- servo calibration
               +---- platform geometry
               +---- PID gains
```

### Why `main.cpp` is small

`main.cpp` defines the robot's control sequence:

1. Read ball position.
2. Run PID.
3. Convert requested platform tilt into three actuator commands.
4. Command the servos.
5. Send low-rate telemetry.

The implementation details stay in the corresponding module.

## Important: this is an INITIAL SOFTWARE SKELETON

The code intentionally does **not** pretend to know the physical geometry
of the 3-arm mechanism.

The touchscreen calibration, servo zero positions, servo directions,
platform geometry, and PID gains must be measured/tuned on the actual robot.

The current `platformTiltToServoAngles()` function in `PID.cpp` uses a
linearized 3-servo mapping as a bring-up model. It is not the final inverse
kinematics of the robot.

## Before running the robot

Open:

```text
include/Config.h
```

and fill in the values marked:

```text
USER INPUT
```

The important items are:

### 1. Touchscreen wiring

Set:

```cpp
TOUCH_XP_PIN
TOUCH_YP_PIN
TOUCH_XM_PIN
TOUCH_YM_PIN
```

The Adafruit library uses the constructor:

```cpp
TouchScreen(XP, YP, XM, YM, resistance)
```

Do not assume the physical labels on your panel correspond directly to
software X/Y. Verify the raw readings experimentally.

### 2. Touchscreen direction

Set:

```cpp
TOUCH_INVERT_X
TOUCH_INVERT_Y
```

to make positive software X/Y correspond to your chosen physical coordinate
system.

### 3. Touchscreen calibration

Measure raw touchscreen values near the four usable edges and replace:

```cpp
TOUCH_RAW_X_MIN
TOUCH_RAW_X_MAX
TOUCH_RAW_Y_MIN
TOUCH_RAW_Y_MAX
```

Then enter the actual usable plate dimensions:

```cpp
PLATE_WIDTH_MM
PLATE_HEIGHT_MM
```

### 4. Servo centers

Determine the angle at which each physical linkage puts the platform in
its true neutral/level position:

```cpp
SERVO_1_CENTER_DEG
SERVO_2_CENTER_DEG
SERVO_3_CENTER_DEG
```

Do not assume 90/90/90.

### 5. Servo directions

Determine whether increasing a servo command produces positive or negative
motion in your mathematical convention:

```cpp
SERVO_1_DIRECTION
SERVO_2_DIRECTION
SERVO_3_DIRECTION
```

Use `+1` or `-1`.

### 6. Servo safety limits

Set:

```cpp
SERVO_MIN_DEG
SERVO_MAX_DEG
```

to conservative values that cannot mechanically bind your mechanism.

### 7. Platform actuator geometry

Enter the approximate actuator azimuths:

```cpp
SERVO_1_AZIMUTH_DEG
SERVO_2_AZIMUTH_DEG
SERVO_3_AZIMUTH_DEG
```

The current linearized model also has:

```cpp
SERVO_TILT_GAIN_DEG
```

This is only a starting calibration.

The final robot should use a proper inverse-kinematics model based on the
actual linkage geometry.

### 8. PID gains

Enter:

```cpp
PID_X_KP
PID_X_KI
PID_X_KD

PID_Y_KP
PID_Y_KI
PID_Y_KD
```

Start with:

```text
Kp = small positive value
Ki = 0
Kd = 0
```

Tune P first. Then add D to reduce oscillation. Add I only when there is a
persistent steady-state offset.

Also verify:

```cpp
PID_X_DIRECTION
PID_Y_DIRECTION
```

If a correction moves the ball farther from the center, the relevant control
direction is wrong.

## Recommended bring-up order

Do not immediately put the ball on the robot.

### Stage 1 - compile

Build the project in PlatformIO.

### Stage 2 - touchscreen only

Verify raw X/Y/Z readings.

Move a conductive object/stylus around the plate and confirm:

- physical +X produces software +X
- physical +Y produces software +Y
- center is approximately `(0,0)`

### Stage 3 - servo only

With the robot mechanically safe:

- command all three servos to neutral
- test one servo at a time
- verify direction
- verify mechanical limits

### Stage 4 - platform mapping

Command small artificial pitch/roll values and verify that the platform
moves in the intended direction.

Do this with the ball removed.

### Stage 5 - PID

Only after the previous stages work should you begin tuning PID.

## Libraries

### Adafruit TouchScreen

This project uses the Adafruit 4-wire resistive touchscreen library.
It is specifically intended for 4-wire resistive touchscreens.

Repository:

https://github.com/adafruit/Adafruit_TouchScreen

### Arduino PID Library

This project uses Brett Beauregard's Arduino PID Library.

Repository:

https://github.com/br3ttb/Arduino-PID-Library

### Arduino Servo

The standard Arduino Servo library is used for the MG996R signal.

Repository:

https://github.com/arduino-libraries/Servo

## Similar project

A particularly relevant open-source reference is:

https://github.com/avrech/ball_on_plate_balancing

That project uses an Arduino-class controller, a touchscreen, MG996R servos,
PID control, and Stewart-platform kinematics. Its documented control loop is
conceptually the same high-level pipeline used here:

```text
touchpad
  ↓
ball x/y
  ↓
PID
  ↓
platform roll/pitch
  ↓
inverse kinematics
  ↓
servo angles
```

Another useful reference is:

https://github.com/JohanLink/Ball-Balancing-PID-System

which is a dedicated ball-balancing PID project with Arduino code and a
3-servo test directory.

## PlatformIO

PlatformIO expects the main C++ source at:

```text
src/main.cpp
```

and the project configuration at:

```text
platformio.ini
```

Typical commands:

```bash
pio run
```

Upload:

```bash
pio run --target upload
```

Serial monitor:

```bash
pio device monitor
```

## Future architecture

Once the basic robot works, the next likely additions are:

- proper 3-arm inverse kinematics
- better touchscreen calibration
- velocity estimation
- derivative filtering
- PID anti-windup
- explicit robot operating modes
- serial command interface
- laptop telemetry/plotting
- emergency stop/fault handling
- automated calibration routines

These should be added only when needed rather than making the initial project
unnecessarily complicated.
