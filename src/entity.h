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
     virtual void setSize(float newSize) { size = newSize; }
     float getSize() const { return size; }
     void accelerate(float amount);
     void rotate(float amount);
     void setRotation(float rotation) { rot = rotation; }
     float getRotation() const { return rot; }
     void setBrake(bool newBrake) { brake = newBrake; }
     bool getBrake() const { return brake; }
     float getSpeed() const { return speed; }
     void setSpeed(float newSpeed) { speed = newSpeed; }
     float getVelocity() const { return std::hypot(deltaX, deltaY); }
     void setBounds(float x, float y) { bounds = {x, y}; }
     Vec2 getBounds() const { return bounds; }
     Hitbox* getHitbox() {return hitbox;}
     virtual void logic();

protected:
     void integrateMotion();
     Game* game;
     float size;
     float rot;
     float speed;
     float deltaX;
     float deltaY;
     float inertia;
     float brakeForce;
     bool brake;
     Vec2 pos;
     Vec2 bounds;
     Hitbox* hitbox;
};