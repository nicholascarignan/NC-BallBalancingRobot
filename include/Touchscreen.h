#pragma once

struct BallPosition
{
    float x_mm;
    float y_mm;
    bool valid;
};

void initializeTouchscreen();
BallPosition getBallPosition();
