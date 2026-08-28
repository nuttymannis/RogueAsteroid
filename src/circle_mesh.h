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
void draw();
void setColor(Vec3 _color) { color = _color; uploadGeometry(); }
void setColor(float _r, float _g, float _b) { color = {_r, _g, _b}; uploadGeometry(); }
~CircleMesh();

private:
GLuint shader;
Vec2 pos;                // Mesh translation sent to the vertex shader.
float size, rot;         // Geometry radius and shader rotation in radians.
void uploadGeometry();
Vec3 color;              // RGB color copied into the color VBO for each point.
unsigned int VAO = 0;    // OpenGL object describing the vertex-input state.
unsigned int vertex_count; // Number of points submitted by glDrawArrays.
std::vector<unsigned int> VBOs; // Position and color buffer object handles.
};
