/**
 * Create meshes to send data to OpenGL.
 */

#pragma once

#include <vector>

class Mesh {

private:
    unsigned vao, vertices_id, index_id, texcoords_id, indices_ct, grass_flag_id;

public:
    Mesh(std::vector<float> vertices, std::vector<int> indices, std::vector<float> texcoords, std::vector<float> grassflags);
    void draw();
    ~Mesh();

};
