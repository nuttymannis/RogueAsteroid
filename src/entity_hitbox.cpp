#include "config.h"
#include "game.h"
#include "entity.h"
#include "entity_hitbox.h"

Hitbox::Hitbox(Game* _game, Entity* _owner){
    game = _game;
    owner = _owner;

    color = {1.0f, 0.0f, 0.0f};

    pos = owner->getPosition();
    size = owner->getSize();
    
    box = {
        pos.x - size,
        pos.y - size,
        size * 2.0f,
        size * 2.0f
    };

    // The mesh uses a local rectangle centered at its origin. The owner
    // position is supplied separately through the mesh transform uniform.
    mesh = new QuadMesh(_game, {
        -size,
        -size,
        size * 2.0f,
        size * 2.0f
    });

    uploadHitbox();
}

Hitbox::Hitbox(Game *_game, Entity* _owner, Rect _r)  : Hitbox(_game, _owner)
{
    setRect(_r);
    mesh->setBox(_r);
}

Hitbox::~Hitbox(){
    delete mesh;
}

bool Hitbox::isColliding(Hitbox* _target)
{
    Rect* _tBox = _target->getRect();

    return  box.x < _tBox->x + _tBox->w &&
            box.x + box.w > _tBox->x &&
            box.y < _tBox->y + _tBox->h &&
            box.y + box.h > _tBox->y;
            
}

void Hitbox::logic(){
    // The hitbox can only update if both its render mesh and owning entity
    // still exist. This also protects against drawing after either object has
    // been destroyed or not initialized.
    if(mesh != nullptr && owner != nullptr){
        drawMesh = game->getDebugStatus();

        // Copy the owner's current transform into the hitbox state so the
        // collision rectangle follows the entity as it moves and rotates.
        pos = owner->getPosition();
        const float ownerSize = owner->getSize();

        // Store the collision rectangle in world coordinates. The rectangle's
        // origin is its upper-left corner, while w and h describe its extent.
        box = {
            pos.x - ownerSize,
            pos.y - ownerSize,
            ownerSize * 2.0f,
            ownerSize * 2.0f
        };

        // QuadMesh stores local vertex coordinates, so give it a rectangle
        // centered around its own origin. The mesh is translated below using
        // the owner's world position.
        mesh->setColor(color);
        mesh->setBox({-ownerSize, -ownerSize, ownerSize * 2.0f, ownerSize * 2.0f});

        // Keep the debug quad aligned with the owner's position and rotation.
        mesh->setPosition(pos.x, pos.y);
        mesh->setRotation(owner->getRotation());
        mesh->setSize(owner->getSize());

        // Draw only the visualization when debug mode is enabled. The actual
        // collision rectangle above is maintained regardless of this flag.
        if(drawMesh)
            draw();
    } else {
        // If the hitbox is no longer valid, disable its debug rendering.
        drawMesh = false;
    }
}

void Hitbox::draw(){
    
    mesh->draw();
}