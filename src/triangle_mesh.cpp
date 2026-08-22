#include "triangle_mesh.h"
#include <vector>
#include <cmath>
#include <math.h>
#include "game.h"
#include <glad/glad.h>

#define M_PI           3.14159265358979323846

TriangleMesh::TriangleMesh(Game* _game) {
    vertex_count = 3;

    // A VAO remembers the vertex-input configuration used by this mesh.
    // The VBOs hold the actual position and color bytes consumed by the GPU.
    glGenVertexArrays(1, &VAO);
    VBOs.resize(2);
    glGenBuffers(2, VBOs.data());

    color = {1.0f, 1.0f, 1.0f};

    uploadGeometry();
}

TriangleMesh::TriangleMesh(Game* _game, float rootX, float rootY, float ship_size) : TriangleMesh(_game) {
    setPosition(rootX, rootY);
    setSize(ship_size);
    uploadGeometry();
}

void TriangleMesh::uploadGeometry() {
    const float positions[] = {
        -size, -size, 0.0f,
        size, -size, 0.0f,
        0.0f, size * 1.75f, 0.0f
    };

    /* const float colors[] = {
        std::fmod(rot/(2.0f*float(M_PI)) + hue, 1.0f), 0.0f, 0.0f,
        0.0f, std::fmod(rot/(2.0f*float(M_PI)) + hue, 1.0f), 0.0f,
        0.0f, 0.0f, std::fmod(rot/(2.0f*float(M_PI)) + hue, 1.0f)
    }; */

    float colors[3 * 3] = {};
    for(size_t i = 0; i < sizeof(colors) / 3 / 4; i++){
        colors[i*3] = color.x;
        colors[i*3+1] = color.y;
        colors[i*3+2] = color.z;
    }

    // Every vertex attribute setup below is recorded in this VAO until another
    // VAO is bound. This lets draw() restore the complete mesh input state with
    // one glBindVertexArray call.
    glBindVertexArray(VAO);

    // Upload position data to buffer 0. The data is static because the mesh
    // shape does not change while it is being rendered.
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // Attribute location 0 receives three floats per vertex: x, y, and z.
    // A stride of 12 bytes equals three 4-byte floats, and offset 0 starts at x.
    glVertexAttribPointer(0, 3, GL_FLOAT, 
            GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(0);

    // Upload one RGB color for each vertex into the second buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    
    // Attribute location 1 receives the three color components. Enabling each
    // attribute makes it available to the vertex shader during drawing.
    glVertexAttribPointer(1, 3, GL_FLOAT, 
            GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(1);
}

void TriangleMesh::draw(GLuint shader) {
    // Reuse the VAO configured during construction. No geometry or GPU buffer
    // is created here, but the mesh is still submitted to the GPU every frame.
    glBindVertexArray(VAO);
    
    // Uniforms are per-draw values rather than per-vertex data. They tell the
    // shader how to rotate and translate this mesh instance for this frame.
    int rotationLocation = glGetUniformLocation(shader, "rotation");
    glUniform1f(rotationLocation, rot);

    int positionLocation = glGetUniformLocation(shader, "position");
    glUniform2f(positionLocation, pos.x, pos.y);

    // Draw the three configured vertices as connected outline segments.
    glLineWidth(2.0f);
    glDrawArrays(GL_LINE_LOOP, 0, vertex_count);
}

TriangleMesh::~TriangleMesh() {
    glDeleteVertexArrays(1, &VAO);
    if (!VBOs.empty()) {
        glDeleteBuffers(static_cast<GLsizei>(VBOs.size()), VBOs.data());
    }
}
