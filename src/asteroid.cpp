#include "config.h"
#include "game.h"
#include "asteroid.h"
#include "asteroid_mesh.h"


Asteroid::Asteroid(Game* _game) : Entity(_game) {
    mesh = new AsteroidMesh(_game);
    pos = {Game::randomFloat(-1.0f, 1.0f),Game::randomFloat(-1.0f, 1.0f)};
    size = Game::randomFloat(0.02f, 0.05f) * 1.5f;
}

Asteroid::Asteroid(Game* _game, float rootX, float rootY, float asteroid_size) : Asteroid(_game) {
    mesh->setPosition(rootX, rootY);
    mesh->setSize(asteroid_size);
    mesh->setColor({Game::randomFloat(0.0f, 1.0f), Game::randomFloat(0.0f, 1.0f), Game::randomFloat(0.0f, 1.0f)});
}

void Asteroid::logic() {
    mesh->setRotation(rot); //((game->deltaTime() * 0.001 * std::sqrt(x)) * (x * x))
    mesh->setSize(size);
}

void Asteroid::draw(){
    Entity::logic();
    Asteroid::logic();
    mesh->setPosition(pos.x, pos.y);
    mesh->draw();
}

void Asteroid::generateMesh(){
    mesh->generateVertexOffsets();
}

