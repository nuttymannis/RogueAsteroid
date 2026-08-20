#include "quad_mesh.h"
#include <vector>
#include <cmath>
#include <math.h>
#include "game.h"
#include <glad/glad.h>

#define M_PI           3.14159265358979323846

QuadMesh::QuadMesh(Game* _game){
    vertex_count = 4;

    shader = _game->getShader();


    color = {1.0f, 1.0f, 1.0f};

    // A VAO remembers the vertex-input configuration used by this mesh.
    // The VBOs hold the actual position and color bytes consumed by the GPU.
    glGenVertexArrays(1, &VAO);
    VBOs.resize(2);
    glGenBuffers(2, VBOs.data());

    uploadGeometry();
}

QuadMesh::QuadMesh(Game* _game, float rootX, float rootY, float _size) : QuadMesh(_game) {
    setPosition(rootX, rootY);
    setSize(_size);
    uploadGeometry();
}

void QuadMesh::uploadGeometry() {
    const float positions[] = {
        -size, size, 0.0f,
        size, size, 0.0f,
        size, -size, 0.0f,
        -size, -size, 0.0f
    };


    float colors[4 * 3] = {};
    for(size_t i = 0; i < 4; i++){
        colors[i*3] = color.x;
        colors[i*3 + 1] = color.y;
        colors[i*3 + 2] = color.z;
    }

    // Every vertex attribute setup below is recorded in this VAO until another
    // VAO is bound. This lets draw() restore the complete mesh input state with
    // one glBindVertexArray call.
    glBindVertexArray(VAO);

    // Upload position data to buffer 0. The data is static because the mesh
    // shape does not change while it is being rendered.
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, 12*4, positions, GL_STATIC_DRAW);

    // Attribute location 0 receives three floats per vertex: x, y, and z.
    // A stride of 16 bytes equals four 4-byte floats, and offset 0 starts at x.
    glVertexAttribPointer(0, 3, GL_FLOAT, 
            GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(0);

    // Upload one RGB color for each vertex into the second buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, 12*4, colors, GL_STATIC_DRAW);
    
    // Attribute location 1 receives the three color components. Enabling each
    // attribute makes it available to the vertex shader during drawing.
    glVertexAttribPointer(1, 3, GL_FLOAT, 
            GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(1);
}

void QuadMesh::draw() {
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

QuadMesh::~QuadMesh() {
    glDeleteVertexArrays(1, &VAO);
    if (!VBOs.empty()) {
        glDeleteBuffers(static_cast<GLsizei>(VBOs.size()), VBOs.data());
    }
}
