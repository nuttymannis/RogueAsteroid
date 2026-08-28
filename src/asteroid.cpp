#include "config.h"
#include "game.h"
#include "asteroid.h"
#include "asteroid_mesh.h"


Asteroid::Asteroid(Game* _game) : Entity(_game) {
    setID(EntityID::Asteroid);
    mesh = new AsteroidMesh(_game);
    pos = {Game::randomFloat(-1.0f, 1.0f),Game::randomFloat(-1.0f, 1.0f)};
    size = Game::randomFloat(0.025f, 0.06f) * 1.5f;
    rot = Game::randomFloat(0, 2*M_PI);
    setAngularVelocity(Game::randomFloat(-2.0f, -2.0f));
}

Asteroid::Asteroid(Game* _game, float rootX, float rootY, float asteroid_size) : Asteroid(_game) {
    pos = {rootX, rootY};
    size = asteroid_size;
    mesh->setPosition(rootX, rootY);
    mesh->setSize(asteroid_size);
    mesh->setColor({Game::randomFloat(0.0f, 1.0f), Game::randomFloat(0.0f, 1.0f), Game::randomFloat(0.0f, 1.0f)});
}

void Asteroid::logic() {
    if(getVelocity() < 0.0015f){
        //setAngularVelocity(Game::randomFloat(-0.5f, 0.5f));
        //accelerate(Game::randomFloat(0.3f, 1.0f));
    }
    mesh->setRotation(rot); //((game->deltaTime() * 0.001 * std::sqrt(x)) * (x * x))
    mesh->setSize(size);
}

void Asteroid::draw(){
    Entity::logic();
    Asteroid::logic();
    mesh->setPosition(pos.x, pos.y);
    mesh->draw();
}

void Asteroid::onCollision(Entity* target){
    if(target != nullptr && target->getID() != EntityID::Bullet && target->getID() != EntityID::Player){
        // Build a vector from the target's center to the asteroid's center.
        Vec2 offset = getPosition() - target->getPosition();

        // The distance is used to normalize offset into a direction vector.
        float distance = offset.magnitude();

        // Avoid dividing by zero if both entities occupy the same position.
        if(distance == 0.0f)
            return;

        // normal points from the target toward the asteroid and has length 1.
        Vec2 normal = offset / distance;

        // Relative velocity describes how the asteroid moves compared with the
        // target, which is what determines whether they approach each other.
        Vec2 relativeVelocity = getVelocityVector() - target->getVelocityVector();

        // The dot product keeps only the relative velocity along the collision
        // normal; sideways motion does not push the objects apart.
        float speedAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

        // A non-negative value means the objects are separating already.
        if(speedAlongNormal >= 0.0f)
            return;

        // Reverse the inward normal speed to create a simple bounce impulse.
        float impulse = -speedAlongNormal;
        // Add the impulse to the asteroid and apply the opposite impulse to the
        // target so momentum is transferred in opposite directions.
        setVelocityVector(getVelocityVector() + normal * impulse); 
        target->setVelocityVector(target->getVelocityVector() - normal * impulse);
    }
}

void Asteroid::generateMesh(){
    mesh->generateVertexOffsets();
}

