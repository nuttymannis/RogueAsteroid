#include "config.h"
#include "game.h"
#include "entity.h"
#include "entity_hitbox.h"

Hitbox::Hitbox(Game* _game, Entity* _owner){
    game = _game;
    owner = _owner;

    pos = {0,0};

    mesh = new QuadMesh(_game);

    uploadHitbox();
}

Hitbox::Hitbox(Game *_game, Entity* _owner, Rect _r)  : Hitbox(_game, _owner)
{
    setRect(_r);
}

bool Hitbox::isColliding(Hitbox* _target)
{
    Rect* _tBox = _target->getRect();

    return ((box.x + box.width() >= _tBox->x) || (box.y + box.height() >= _tBox->y));
}

void Hitbox::logic(){
    pos = owner->getPosition();
    setRect({pos.x - size, pos.y + size, pos.x + size, pos.y - size});
    mesh->setPosition(pos.x, pos.y);
    mesh->setRotation(owner->getRotation());
    mesh->setSize(owner->getSize());

    if(drawMesh)
        draw();
}

void Hitbox::draw(){
    
    mesh->draw();
}