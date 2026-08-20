#pragma once

#include "config.h"
#include "game.h"
#include "entity.h"
#include "quad_mesh.h"

class Game;

class QuadMesh;

struct Rect {
    float x, y, x1, y1;

    float width() {return x1-x;}
    float height() {return y1-y;}

    Vec2 getPos() {return {x + width()/2, y - height()/2};}
};

class Hitbox {
    public:
    Hitbox(Game* _game, Entity* _owner);
    Hitbox(Game *_game, Entity* _owner, Rect _r);

    void setRect(Rect _r)     {box = _r;}
    Rect* getRect()           {return &box;}
    Entity* getOwner()        {return owner;}

    void toggleDraw()         {drawMesh = !drawMesh;}

    void uploadHitbox()       
    {
        game->getHitboxList().push_back(this); 
        printf("Uploaded Hitbox: [%d](%p) %p\n", static_cast<int>(game->getHitboxList().size()), owner, this);
    }

    bool isColliding(Hitbox* _target);

    void logic();
    void draw();

    private:
    bool drawMesh = true;
    QuadMesh* mesh;
    Vec3 color;
    float size = 5.0f;
    Entity* owner;
    Game* game;
    Rect box;
    Vec2 pos;
};

