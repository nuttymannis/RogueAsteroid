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
    
    worldBox = {
        pos.x - size,
        pos.y - size,
        size * 2.0f,
        size * 2.0f
    };

    // The hitboxMesh uses a local rectangle centered at its origin. The owner
    // position is supplied separately through the hitboxMesh transform uniform.
    hitboxMesh = new QuadMesh(_game, {
        -size,
        -size,
        size * 2.0f,
        size * 2.0f
    });

    accelMesh = new QuadMesh(_game, {
        1.0f,
        1.0f,
        1.0f,
        1.0f
    });

    forwardMesh = new QuadMesh(_game, {
        1.0f,
        1.0f,
        1.0f,
        1.0f
    });

    uploadHitbox();
}

Hitbox::Hitbox(Game *_game, Entity* _owner, Rect _r)  : Hitbox(_game, _owner)
{
    setRect(_r);
    hitboxMesh->setBox(_r);
}

Hitbox::~Hitbox(){
    if (game != nullptr) {
        game->removeHitbox(this);
    }
    delete hitboxMesh;
    delete forwardMesh;
    delete accelMesh;
}

bool Hitbox::isColliding(Hitbox* _target)
{
    if (_target == nullptr)
        return false;

    Rect* _tBox = _target->getRect();

    return  worldBox.x < _tBox->x + _tBox->w &&
            worldBox.x + worldBox.w > _tBox->x &&
            worldBox.y < _tBox->y + _tBox->h &&
            worldBox.y + worldBox.h > _tBox->y;
            
}

void Hitbox::logic(){
    // The hitbox can only update if both its render hitboxMesh and owning entity
    // still exist. This also protects against drawing after either object has
    // been destroyed or not initialized.
    if(hitboxMesh != nullptr && owner != nullptr && game != nullptr){
        drawMesh = game->getDebugStatus();

        // Copy the owner's current transform into the hitbox state so the
        // collision rectangle follows the entity as it moves and rotates.
        pos = owner->getPosition();
        const float ownerSize = owner->getSize();

        // Store the collision rectangle in world coordinates. The rectangle's
        // origin is its upper-left corner, while w and h describe its extent.
        worldBox = {
            pos.x - ownerSize,
            pos.y - ownerSize,
            ownerSize * 2.0f,
            ownerSize * 2.0f
        };

        // QuadMesh stores local vertex coordinates, so give it a rectangle
        // centered around its own origin. The hitboxMesh is translated below using
        // the owner's world position.

        Rect localBox = {-ownerSize, -ownerSize, worldBox.w, worldBox.h}; // Stores world hitbox coordinates in local coordinates
        hitboxMesh->setBox(localBox);
        hitboxMesh->setColor(color);
        
        float tanX = std::sin(owner->getAcceleration().x);
        float tanY = std::cos(owner->getAcceleration().y);

        // Keep the debug quad aligned with the owner's position and rotation.
        hitboxMesh->setPosition(pos.x, pos.y);
        hitboxMesh->setRotation(owner->getRotation());
        hitboxMesh->setSize(owner->getSize());


        Rect linebox = {
            -ownerSize + (worldBox.w / 2.0f) - MESH_WIDTH,
            -ownerSize + (worldBox.h / 2.0f),
            MESH_WIDTH * 2.0f,
            0.1f
        };

        forwardMesh->setBox(linebox);
        forwardMesh->setColor(Vec3{1.0f,1.0f,1.0f});

        forwardMesh->setPosition(worldBox.getPos().x, worldBox.getPos().y);

        /* forwardMesh->setRotation(std::acos(
            owner->getPosition().dot(Vec2{tanX, tanY}) /
            (owner->getPosition().magnitude() * Vec2{tanX, tanY}.magnitude())
        )); // -std::atan2(tanY, tanX) */
        forwardMesh->setRotation(owner->getRotation());
        forwardMesh->setSize(owner->getSize());

        linebox.h = 0.01f + ownerSize * 400.0f * owner->getVelocity();
        accelMesh->setBox(linebox);
        accelMesh->setColor(Vec3{0.0f,1.0f,0.0f});

        accelMesh->setPosition(worldBox.getPos().x, worldBox.getPos().y);

        /* accelMesh->setRotation(std::acos(
            owner->getPosition().dot(Vec2{tanX, tanY}) /
            (owner->getPosition().magnitude() * Vec2{tanX, tanY}.magnitude())
        )); // -std::atan2(tanY, tanX) */
        accelMesh->setRotation(forwardMeshRot);
        accelMesh->setSize(owner->getSize());

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
    hitboxMesh->draw();
    forwardMesh->draw();
    accelMesh->draw();
}