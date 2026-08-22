#pragma once

#include "config.h"

class Game;
class Hitbox;

// Entity contains shared simulation state. Rendering remains in the concrete
// mesh classes that derive from it or are owned by higher-level entities.
class Entity {
public:
     explicit Entity(Game* game, bool generateHitbox = true);
     virtual ~Entity();

     void setPosition(float x, float y) { pos = {x, y}; }
     Vec2 getPosition() const { return pos; }
     Vec2 forward() const { return {std::sin(rot), std::cos(rot)}; }
     virtual void setSize(float newSize) { size = newSize; }
     virtual void onCollision(Entity* target) {}//printf("Entity *%p collided with Entity *%p\n", this, target);
     float getSize() const { return size; }
     void accelerate(float amount);
     void rotate(float amount);
     void setRotation(float rotation) { rot = rotation; }
     void setAngularVelocity(float deltaRotation) { deltaRot = deltaRotation; printf("Entity *%p angular velocity set to %f\n", this, deltaRot); }
     void setAcceleration(Vec2 _accel) {deltaX = _accel.x; deltaY = _accel.y;}
     Vec2 getAcceleration() {return Vec2{deltaX, deltaY};}
     float getRotation() const { return rot; }
     void setBrake(bool newBrake) { brake = newBrake; }
     void kill() { deathFlag = true; }
     bool isDead() const { return deathFlag; }
     bool getBrake() const { return brake; }
     float getSpeed() const { return speed; }
     void setSpeed(float newSpeed) { speed = newSpeed; }
     float getVelocity() const { return std::hypot(deltaX, deltaY); }
     void setBounds(float x, float y) { bounds = {x, y}; }
     Vec2 getBounds() const { return bounds; }
     Hitbox* getHitbox() const { return hitbox; }
     virtual void logic();

protected:
     void integrateMotion();
     Game* game;          // Game instance that supplies timing and shared systems.
     float size;          // Entity radius or visual size in normalized coordinates.
     float rot;           // Facing angle in radians.
     float deltaRot = 0;  // Angular velocity in radians.
     float speed;         // Base movement or launch speed used by the entity.
     float deltaX;        // Current horizontal velocity/displacement component.
     float deltaY;        // Current vertical velocity/displacement component.
     float inertia;       // Per-update damping applied to velocity.
     float brakeForce;    // Additional damping applied while braking.
     bool brake;          // Whether braking behavior is currently active.
     bool deathFlag;      // Whether the entity is marked for removal from the game world.
     Vec2 pos;            // Entity position in normalized OpenGL coordinates.
     Vec2 bounds;         // Configured world bounds for the entity.
     Hitbox* hitbox;      // Optional collision box owned by this entity.
};