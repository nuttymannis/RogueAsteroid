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
    void draw() override;
    void fireWeapon();
    void input();
    void onCollision(Entity* target) override;
    InputBuffer* getInputBuffer()   {return inputBuffer;}
    
    
    private:
    GLuint shader;
    TriangleMesh* ship;
    InputBuffer* inputBuffer;
    Vec3 playerColor;
    int hp;
    float weaponCooldown, lastTime, currentTime;
    float playerHue, hueIncrement = 0.5f;
    bool rainbow = true;
    bool rainbowIncreasing = true;
};