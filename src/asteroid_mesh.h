#pragma once
#include "entity.h"

class Game;

class AsteroidMesh : public Entity {
public:
AsteroidMesh(Game* _game);
AsteroidMesh(Game* _game, float rootX, float rootY, float ship_size);
void setSize(float newSize) override {
	size = newSize;
	uploadGeometry();
}
void draw();
void setColor(Vec3 _color) { color = _color; uploadGeometry(); }
void setColor(float _r, float _g, float _b) { color = {_r, _g, _b}; uploadGeometry(); }
void generateVertexOffsets();
~AsteroidMesh();

private:
void uploadGeometry();
std::vector<float> asteroidVertexOffests;
Vec3 color;
unsigned int VAO = 0, vertex_count;
std::vector<unsigned int> VBOs;
GLuint shader;
};
