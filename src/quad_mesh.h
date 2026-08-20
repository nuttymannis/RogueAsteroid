#pragma once
#include "config.h"
#include "entity_hitbox.h"


class Game;

class QuadMesh {
public:
QuadMesh(Game* _game);
QuadMesh(Game* _game, float rootX, float rootY, float _size);
void setSize(float newSize) {
	size = newSize;
	uploadGeometry();
}
void setPosition(float x, float y) { pos = {x, y}; }
void setRotation(float rotation) { rot = rotation; }
void setColor(Vec3 _color) {color = _color;}
Vec3 getColor() {return color;}
void draw();
~QuadMesh();

private:
void uploadGeometry();
Vec2 pos;
Vec3 color;
GLuint shader;
float size = 0.03f, rot = 0.0f;
unsigned int VAO = 0, vertex_count;
std::vector<unsigned int> VBOs;
};
