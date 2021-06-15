/**
 * Create meshes to send data to OpenGL.
 */

#pragma once

#include <vector>

class Mesh {

private:
    unsigned vao, vertices_id, index_id, color_id, indices_ct;

public:
    Mesh(std::vector<float> vertices, std::vector<int> indices, std::vector<float> colors);
    void draw();
    ~Mesh();

};
