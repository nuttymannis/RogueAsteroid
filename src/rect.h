#pragma once

#include "config.h"

struct Rect {
    float x, y, w, h;

    float width() const { return w; }
    float height() const { return h; }
    Vec2 getPos() const { return {x + w / 2.0f, y + h / 2.0f}; }
};