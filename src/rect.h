#pragma once

#include "config.h"
#include <array>

struct Rect {
    float x, y, w, h;

    float width() const { return w; }
    float height() const { return h; }
    Vec2 getPos() const { return {x + w / 2.0f, y + h / 2.0f}; }

    std::array<Vec2, 4> vertices(float rotation) const {
        // Rotate local corner offsets around the rectangle's world-space
        // center instead of rotating the rectangle around the origin.
        Vec2 pos = getPos();

        // Store each corner relative to the center. The rectangle is centered
        // at (0, 0) here, so rotation can be applied before translation.
        Vec2 corners[4] = {
            {-w / 2.0f, -h / 2.0f},
            { w / 2.0f, -h / 2.0f},
            { w / 2.0f,  h / 2.0f},
            {-w / 2.0f,  h / 2.0f}
        };

        // Rotation uses the standard 2D rotation matrix:
        // x' = x cos(r) - y sin(r), y' = x sin(r) + y cos(r).
        float cosine = std::cos(rotation);
        float sine = std::sin(rotation);

        // Hold the final corner positions in world coordinates.
        std::array<Vec2, 4> rotated;

        // Rotate each local corner, then add the rectangle center to translate
        // it back into the same coordinate space as the other game objects.
        for (int i = 0; i < 4; ++i) {
            rotated[i] = {
                pos.x + corners[i].x * cosine - corners[i].y * sine,
                pos.y + corners[i].x * sine + corners[i].y * cosine
            };
        }

        return rotated;
    }
};