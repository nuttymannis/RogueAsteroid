#pragma once
#include "entity.h"

class Game;

class CircleMesh {
public:
CircleMesh(Game* _game);
CircleMesh(Game* _game, float rootX, float rootY, float ship_size);
void setSize(float newSize) {
	size = newSize;
	uploadGeometry();
}
void setPosition(float _x, float _y) {pos = {_x, _y};}
void setRotation(float r) {rot = r;}
void draw(GLuint shader);
void setColor(Vec3 _color) { color = _color; uploadGeometry(); }
void setColor(float _r, float _g, float _b) { color = {_r, _g, _b}; uploadGeometry(); }
~CircleMesh();

private:
Vec2 pos;
float size, rot;
void uploadGeometry();
Vec3 color;
unsigned int VAO = 0, vertex_count;
std::vector<unsigned int> VBOs;
};
