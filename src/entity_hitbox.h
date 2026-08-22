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

    void setRect(Rect _r)     {worldBox = _r;}
    void setColor(Vec3 _color){color = _color;}
    Rect* getRect()           {return &worldBox;}
    Entity* getOwner()        {return owner;}

    Vec2 getPos()             {return pos;}
    void setPos(Vec2 _pos)    {pos = _pos;}

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

    float forwardMeshRot = 0.0f;

    private:
    const float MESH_WIDTH = 0.001f;
    float rot;
    bool drawMesh = true;
    QuadMesh* hitboxMesh;
    QuadMesh* accelMesh;
    QuadMesh* forwardMesh;
    Vec3 color;
    float size = 5.0f;
    Entity* owner;
    Game* game;
    Rect worldBox;
    Vec2 pos;
};

