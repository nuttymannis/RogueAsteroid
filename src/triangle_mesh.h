#pragma once
#include "config.h"

class Game;

class TriangleMesh {
public:
TriangleMesh(Game* _game);
TriangleMesh(Game* _game, float rootX, float rootY, float ship_size);
void setSize(float newSize) {
	size = newSize;
	uploadGeometry();
}
void setPosition(float x, float y) { pos = {x, y}; }
void setRotation(float rotation) { rot = rotation; }
void draw(GLuint shader);
~TriangleMesh();

private:
void uploadGeometry();
Vec2 pos;
float size = 0.03f, rot = 0.0f;
unsigned int VAO = 0, vertex_count;
std::vector<unsigned int> VBOs;
};
