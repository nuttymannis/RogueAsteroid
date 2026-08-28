#pragma once

#include "config.h"
#include "entity.h"

class CircleMesh;

class Star : public Entity {
    public:
    Star(Game* _game, bool init = true);
    Star(Game* _game, float rootX, float rootY, float star_size);
    Star(Game* _game, float star_size, float _bright = 1.0f);
    float getBrightness() {return brightness;}
    void setBrightness(float _brightness) {brightness = _brightness;}
    void randomizePosition();
    void logic() override {}
    void draw() override;

    private:
    void initMesh();
    float brightness;
    CircleMesh* mesh;
};