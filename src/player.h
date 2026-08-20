#pragma once
#include "config.h"
#include "entity.h"
#include "triangle_mesh.h"

class Game;

class Player : public Entity{
    public:
    Player(Game* _game, GLuint _shader);
    TriangleMesh* getShip() { return ship; }
    void draw();
    void fireWeapon();
    void input();
    
    
    private:
    GLuint shader;
    TriangleMesh* ship;
    int hp;
    float weaponCooldown, lastTime, currentTime;
};