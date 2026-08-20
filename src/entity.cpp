#include "entity.h"
#include "game.h"
#include "entity_hitbox.h"

Entity::Entity(Game* _game, bool generateHitbox)
    : game(_game),
      size(0.03f),
      rot(0.0f),
      speed(0.40f),
      deltaX(0.0f),
      deltaY(0.0f),
      inertia(0.0005f),
      brakeForce(0.01f),
      brake(false),
      pos{0.0f, 0.0f},
      bounds{800.0f, 600.0f}
{
    if(generateHitbox)
        hitbox = new Hitbox(_game, this);
}

Entity::~Entity()
{
    delete hitbox;
}

void Entity::accelerate(float amount)
{
    // The mesh points upward when rotation is zero. Convert that heading into
    // a direction vector, then scale it by the requested acceleration and the
    // elapsed frame time so movement is independent of frame rate.
    const float forwardX = std::sin(rot);
    const float forwardY = std::cos(rot);
    deltaX += forwardX * amount * game->deltaTime();
    deltaY += forwardY * amount * game->deltaTime();
}

void Entity::rotate(float amount)
{
    rot += amount;
}

void Entity::logic()
{
    hitbox->logic();
    integrateMotion();

    // World entities wrap around the normalized screen bounds by default.
    // Projectiles override logic() so they can leave the screen and expire.
    if (pos.x > 1.0f || pos.x < -1.0f) {
        pos.x *= -1.0f;
        pos.y *= -1.0f;
    }
    if (pos.y > 1.0f || pos.y < -1.0f) {
        pos.y *= -1.0f;
        pos.x *= -1.0f;
    }
}

void Entity::integrateMotion()
{
    // Apply drag before moving. This creates gradual slowing rather than an
    // immediate stop, and braking applies an additional damping factor.
    deltaX *= 1.0f - inertia;
    deltaY *= 1.0f - inertia;

    if (brake) {
        deltaX *= 1.0f - brakeForce;
        deltaY *= 1.0f - brakeForce;
    }

    pos.x += deltaX;
    pos.y += deltaY;

}
