#pragma once

#include "config.h"
#include "game.h"
#include "entity.h"
#include "quad_mesh.h"
#include "rect.h"

class Game;

class QuadMesh;

class Hitbox {
    public:
    Hitbox(Game* _game, Entity* _owner);
    Hitbox(Game *_game, Entity* _owner, Rect _r);
    ~Hitbox();

    void setRect(Rect _r)     {box = _r;}
    void setColor(Vec3 _color){color = _color;}
    Rect* getRect()           {return &box;}
    Entity* getOwner()        {return owner;}

    void toggleDraw()         {drawMesh = !drawMesh;}
    void setDraw(bool _draw)  {drawMesh = _draw;}

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

