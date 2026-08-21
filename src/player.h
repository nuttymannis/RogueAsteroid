#pragma once
#include "config.h"
#include "entity.h"
#include "input_buffer.h"
#include "triangle_mesh.h"

class Game;

class InputBuffer;

class Player : public Entity{
    public:
    Player(Game* _game, GLuint _shader);
    TriangleMesh* getShip() { return ship; }
    void draw();
    void fireWeapon();
    void input();
    InputBuffer* getInputBuffer()   {return inputBuffer;}
    
    
    private:
    GLuint shader;
    TriangleMesh* ship;
    InputBuffer* inputBuffer;
    int hp;
    float weaponCooldown, lastTime, currentTime;
};