#pragma once

struct RPRect
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

    int left() const { return x; }
    int right() const { return x + width; }
    int top() const { return y; }
    int bottom() const { return y + height; }
};