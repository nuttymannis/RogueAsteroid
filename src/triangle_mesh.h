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
void setColor(Vec3 _color) {color = _color; uploadGeometry();}
void setPosition(float x, float y) { pos = {x, y}; }
void setRotation(float rotation) { rot = rotation; }
void draw(GLuint shader);
~TriangleMesh();

private:
void uploadGeometry();
Vec2 pos;                    // Mesh translation sent to the vertex shader.
Vec3 color;
float size = 0.03f;          // Triangle size used when building its vertices.
float rot = 0.0f;            // Mesh rotation in radians.
unsigned int VAO = 0;        // OpenGL vertex-array configuration handle.
unsigned int vertex_count;   // Number of triangle vertices submitted.
std::vector<unsigned int> VBOs; // Position and color buffer object handles.
};
