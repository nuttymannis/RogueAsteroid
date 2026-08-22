#include "config.h"
#include "star.h"
#include "circle_mesh.h"
#include "game.h"

Star::Star(Game* _game, bool init) : Entity(_game, false){
    randomizePosition();
    size = 0.005f;
    brightness = 1;
    if(init)
    initMesh();
}

Star::Star(Game* _game, float star_size, float _bright) : Star(_game, false){
    size = star_size;
    brightness = _bright;
    initMesh();
}

void Star::draw(GLuint _shader)
{
    mesh->setPosition(pos.x, pos.y);
    mesh->setRotation(rot);
    mesh->draw(_shader);
}

Star::Star(Game* _game, float rootX, float rootY,
           float star_size) : Star(_game, false){
    setPosition(rootX, rootY);
    setSize(star_size);
    initMesh();
}

void Star::randomizePosition(){
    // Stars are placed directly in clip-space coordinates, so each coordinate
    // is sampled from the range visible to the vertex shader.
    pos = {Game::randomFloat(-1.0f, 1.0f), Game::randomFloat(-1.0f, 1.0f)};
}

void Star::initMesh(){
    mesh = new CircleMesh(game);
    mesh->setPosition(pos.x, pos.y);
    mesh->setColor(brightness, brightness, brightness);
    // Copy the star's small inherited radius into the render mesh once during
    // initialization; the mesh does not use its own simulation state.
    mesh->setSize(getSize());
}