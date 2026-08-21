#pragma once
#include "config.h"
#include "rect.h"


class Game;

class QuadMesh {
public:
QuadMesh(Game* _game, Rect _r);
QuadMesh(Game* _game, float rootX, float rootY, float _size);
QuadMesh(Game* _game, float _size);
void setSize(float newSize) {
	size = newSize;
	uploadGeometry();
}
void setBox(Rect newBox) {
	box = newBox;
	// The rectangle is vertex data, so upload the new coordinates after
	// changing it rather than waiting for a later draw call.
	uploadGeometry();
}
void setPosition(float x, float y) { pos = {x, y}; }
void setRotation(float rotation) { rot = rotation; }
void setColor(Vec3 _color) {color = _color;}
Vec3 getColor() {return color;}
void draw();
~QuadMesh();

private:
Rect box;                  // Rectangle bounds used to construct the quad geometry.
void uploadGeometry();
Vec2 pos;                    // Rectangle center/translation in normalized space.
Vec3 color;                  // Color uploaded for each rectangle vertex.
GLuint shader;               // Shader program used when drawing the quad.
float size = 0.03f;          // Half-size used to construct the quad geometry.
float rot = 0.0f;            // Quad rotation in radians.
unsigned int VAO = 0;        // OpenGL vertex-array configuration handle.
unsigned int vertex_count;   // Number of vertices submitted for the quad.
std::vector<unsigned int> VBOs; // Position and color buffer object handles.
};
