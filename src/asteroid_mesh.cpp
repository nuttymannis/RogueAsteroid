#include "asteroid_mesh.h"
#include <vector>
#include <cmath>
#include <math.h>
#include "game.h"
#include <glad/glad.h>

#define M_PI           3.14159265358979323846

AsteroidMesh::AsteroidMesh(Game* _game) : Entity(_game){
    vertex_count = 12;

    color = {1.0f, 1.0f, 1.0f};
    
    shader = _game->getShader();

    // The VAO stores the circle's vertex-input configuration, while the two
    // VBOs store its point positions and per-point colors on the GPU.
    glGenVertexArrays(1, &VAO);
    VBOs.resize(2);
    glGenBuffers(2, VBOs.data());

    // Geometry uses one random offset per point, so generate those values
    // before the first upload instead of indexing an empty vector.
    generateVertexOffsets();
    uploadGeometry();
}

AsteroidMesh::AsteroidMesh(Game* _game, float rootX, float rootY, float asteroid_size) : AsteroidMesh(_game) {
    setPosition(rootX, rootY);
    Entity::setSize(asteroid_size);
    uploadGeometry();
}

void AsteroidMesh::generateVertexOffsets() {
    asteroidVertexOffests.clear();
    asteroidVertexOffests.reserve(16);

    for(int i = 0; i < 16; i++){
        asteroidVertexOffests.push_back(Game::randomFloat(0.4f, 1.1f));
    }

    // Rebuild the GPU vertex buffer so newly generated offsets affect the
    // rendered asteroid immediately.
    if (!asteroidVertexOffests.empty()) {
        uploadGeometry();
    }
}

void AsteroidMesh::uploadGeometry() {
    // Store 12 points around the circle. Each point uses three consecutive
    // floats in the format x, y, z because the vertex shader expects vec3 data.
    float positions[12 * 3];
    float offset = 0;
    for (int point = 0; point < 12; ++point) {
        // A full circle contains 2*pi radians. Dividing it into 12 equal
        // sections gives each point the same angular distance from the next.
        const float angle = (2.0f * static_cast<float>(M_PI) * point) / 12.0f;
        
        // cos(angle) and sin(angle) produce a point on a unit circle. Scaling
        // both coordinates by size changes the radius without changing the
        // circle's shape. The point's z coordinate remains on the 2D plane.
        
        positions[point * 3] = std::cos(angle) * size
                                               * asteroidVertexOffests[point % asteroidVertexOffests.size()];
        positions[point * 3 + 1] = std::sin(angle) * size;
        positions[point * 3 + 2] = 0.0f;
    }

    float colors[12 * 3] = {};
    for(int i = 0; i < 12; ++i){
        // Colors are stored in a separate VBO but use the same one-RGB-triplet
        // per vertex layout as the position buffer's one-XYZ-triplet layout.
        colors[i * 3] = color.x;
        colors[i * 3 + 1] = color.y;
        colors[i * 3 + 2] = color.z;
    }

    // Record the attribute configuration in this VAO so draw() can reuse it.
    glBindVertexArray(VAO);

    // Upload the generated circle coordinates once as static vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    // Location 0 matches vertexPos in the vertex shader. Each point is an
    // interleaved-looking sequence of three floats: x, y, and z.
    glVertexAttribPointer(0, 3, GL_FLOAT, 
            GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(0);

    // Upload the RGB color associated with each of the 12 circle points.
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    // Location 1 matches vertexColor in the vertex shader.
    glVertexAttribPointer(1, 3, GL_FLOAT, 
            GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(1);
}

void AsteroidMesh::draw() {
    // Rebind the previously configured VAO. The circle is still rendered every
    // frame; only its changing transform values need to be sent again.
    glBindVertexArray(VAO);
    
    // Send the current rotation and position to the active shader program.
    int rotationLocation = glGetUniformLocation(shader, "rotation");
    glUniform1f(rotationLocation, rot);

    int positionLocation = glGetUniformLocation(shader, "position");
    glUniform2f(positionLocation, pos.x, pos.y);

    glLineWidth(2.0f);
    // GL_LINE_LOOP connects each point to the next and closes the final point
    // back to the first, producing the circle outline.
    glDrawArrays(GL_LINE_LOOP, 0, vertex_count);
}

AsteroidMesh::~AsteroidMesh() {
    glDeleteVertexArrays(1, &VAO);
    if (!VBOs.empty()) {
        glDeleteBuffers(static_cast<GLsizei>(VBOs.size()), VBOs.data());
    }
}
