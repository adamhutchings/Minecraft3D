#include "chunk.hpp"

#include <vector>

namespace {

// Get the OpenGL vertices of a block face
std::vector<float> get_face_vertices(
	Side side, float x, float y, float z
) {
    switch (side) {
        case UP:
            return std::vector<float> {
                x + 1, y + 1, z,
                x,     y + 1, z,
                x,     y + 1, z + 1,
                x + 1, y + 1, z + 1,
            };
        case DOWN:
            return std::vector<float> {
                x + 1, y, z,
                x + 1, y, z + 1,
                x,     y, z + 1,
                x,     y, z,
            };
        case WEST:
            return std::vector<float> {
                x, y,     z + 1,
                x, y + 1, z + 1,
                x, y + 1, z,
                x, y,     z,
            };
        case EAST:
            return std::vector<float> {
                x + 1, y,     z,
                x + 1, y + 1, z,
                x + 1, y + 1, z + 1,
                x + 1, y,     z + 1,
            };
        case NORTH:
            return std::vector<float> {
                x,     y,     z,
                x,     y + 1, z,
                x + 1, y + 1, z,
                x + 1, y,     z,
            };
        case SOUTH:
            return std::vector<float> {
                x + 1, y,     z + 1,
                x + 1, y + 1, z + 1,
                x,     y + 1, z + 1,
                x,     y,     z + 1,
            };
        default:
            return std::vector<float> {};
    }
}

}

Chunk::Chunk() {

	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			for (int k = 0; k < CHUNK_SIZE; ++k) {
				this->at(i, j, k) = GRASS_BLOCK; // for now
			}
		}
	}

	update_mesh();

}

BlockType& Chunk::at(int x, int y, int z) {
	return blocks.at(x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z);
}

bool Chunk::block_borders_air(int i, int j, int k, Side side) {
	switch (side) {
		case UP:
			if (j == CHUNK_SIZE - 1) return true;
			return at(i, j + 1, k) == AIR_BLOCK;
		case DOWN:
			if (j == 0) return true;
			return at(i, j - 1, k) == AIR_BLOCK;
		case WEST:
			if (i == 0) return true;
			return at(i - 1, j, k) == AIR_BLOCK;
		case EAST:
			if (i == CHUNK_SIZE - 1) return true;
			return at(i + 1, j, k) == AIR_BLOCK;
		case NORTH:
			if (k == 0) return true;
			return at(i, j, k - 1) == AIR_BLOCK;
		case SOUTH:
			if (k == CHUNK_SIZE - 1) return true;
			return at(i, j, k + 1) == AIR_BLOCK;
	}
}

void Chunk::update_mesh() {

	std::vector<float> vertices;
    std::vector<int> indices;
    std::vector<float> texcoords;

    int faces_pushed = 0;

    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            for (int k = 0; k < CHUNK_SIZE; ++k) {
                
                for (int s = 0; s < 6; ++s) {
                    Side side = static_cast<Side>(s);
                    if (block_borders_air(
                        i, j, k, side
                    ) && this->at(
                        i, j, k
                    ) != AIR_BLOCK) {

                    	auto block = at(i, j, k);

                        // Add block face to the mesh
                        for (auto vertex : get_face_vertices(side, i, j, k)) {
                            vertices.push_back(vertex);
                        }

                        Texture tex = blocks::get_texture(block, side);

                        std::vector<float> coords = tex::get_vertices(tex);

                        for (auto f : coords)
                            texcoords.push_back(f);

                        std::array<int, 6> indices_{ 0, 1, 3, 3, 1, 2 };
                        for (auto i : indices_) {
                            indices.push_back(i + faces_pushed * 4);
                        }

                        ++faces_pushed;

                    }
                }

            }
        }
    }

   	chunk_mesh = std::make_unique<Mesh>(vertices, indices, texcoords);

}

void Chunk::render() {
	chunk_mesh->draw();
}
