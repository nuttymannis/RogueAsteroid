#pragma once

#include "config.h"
#include "entity.h"
#include "asteroid_mesh.h"

class AsteroidMesh;

class Asteroid : public Entity {
    public:
    Asteroid(Game* _game);
    Asteroid(Game* _game, float rootX, float rootY, float asteroid_size);
    
    void logic() override;

    void onCollision(Entity* target) override;

    void generateMesh();

    void draw() override;

    private:
    AsteroidMesh* mesh;
    GLuint shader;
};