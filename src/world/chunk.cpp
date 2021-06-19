#include "chunk.hpp"

#include <vector>

#include <world/world.hpp>

namespace {

// Get the OpenGL vertices of a block face
std::vector<float> get_face_vertices(
	Side side, float x, float y, float z,
    float cx, float cy, float cz // the chunk coordinates
) {
    float finalX = cx * CHUNK_SIZE + x;
    float finalY = cy * CHUNK_SIZE + y;
    float finalZ = cz * CHUNK_SIZE + z;
    switch (side) {
        case UP:
            return std::vector<float> {
                finalX + 1, finalY + 1, finalZ,
                finalX,     finalY + 1, finalZ,
                finalX,     finalY + 1, finalZ + 1,
                finalX + 1, finalY + 1, finalZ + 1,
            };
        case DOWN:
            return std::vector<float> {
                finalX + 1, finalY, finalZ,
                finalX + 1, finalY, finalZ + 1,
                finalX,     finalY, finalZ + 1,
                finalX,     finalY, finalZ,
            };
        case WEST:
            return std::vector<float> {
                finalX, finalY,     finalZ + 1,
                finalX, finalY + 1, finalZ + 1,
                finalX, finalY + 1, finalZ,
                finalX, finalY,     finalZ,
            };
        case EAST:
            return std::vector<float> {
                finalX + 1, finalY,     finalZ,
                finalX + 1, finalY + 1, finalZ,
                finalX + 1, finalY + 1, finalZ + 1,
                finalX + 1, finalY,     finalZ + 1,
            };
        case NORTH:
            return std::vector<float> {
                finalX,     finalY,     finalZ,
                finalX,     finalY + 1, finalZ,
                finalX + 1, finalY + 1, finalZ,
                finalX + 1, finalY,     finalZ,
            };
        case SOUTH:
            return std::vector<float> {
                finalX + 1, finalY,     finalZ + 1,
                finalX + 1, finalY + 1, finalZ + 1,
                finalX,     finalY + 1, finalZ + 1,
                finalX,     finalY,     finalZ + 1,
            };
        default:
            return std::vector<float> {};
    }
}

}

Chunk::Chunk(int x, int y, int z) {

    cx = x, cy = y, cz = z;

	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			for (int k = 0; k < CHUNK_SIZE; ++k) {
				this->at(i, j, k) = GRASS_BLOCK; // for now
			}
		}
	}

}

BlockType& Chunk::at(int x, int y, int z) {
	return blocks.at(x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z);
}

bool Chunk::block_borders_air(int i, int j, int k, Side side, World* world) {
	switch (side) {
		case UP:
			return world->block_at(i, j + 1, k) == AIR_BLOCK;
		case DOWN:
			return world->block_at(i, j - 1, k) == AIR_BLOCK;
		case WEST:
			return world->block_at(i - 1, j, k) == AIR_BLOCK;
		case EAST:
			return world->block_at(i + 1, j, k) == AIR_BLOCK;
		case NORTH:
			return world->block_at(i, j, k - 1) == AIR_BLOCK;
		case SOUTH:
			return world->block_at(i, j, k + 1) == AIR_BLOCK;
	}
}

void Chunk::update_mesh(World* world) {

	std::vector<float> vertices;
    std::vector<int> indices;
    std::vector<float> texcoords;
    std::vector<float> grassflags;

    int faces_pushed = 0;

    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            for (int k = 0; k < CHUNK_SIZE; ++k) {
                
                for (int s = 0; s < 6; ++s) {
                    Side side = static_cast<Side>(s);
                    if (block_borders_air(
                        cx * CHUNK_SIZE + i,
                        cy * CHUNK_SIZE + j,
                        cz * CHUNK_SIZE + k,
                        side, world
                    ) && this->at(
                        i, j, k
                    ) != AIR_BLOCK) {

                    	auto block = at(i, j, k);

                        // Add block face to the mesh
                        for (auto vertex : get_face_vertices(side, i, j, k, cx, cy, cz)) {
                            vertices.push_back(vertex);
                        }

                        Texture tex = blocks::get_texture(block, side);

                        std::vector<float> coords = tex::get_vertices(tex);

                        float grass_flag = 0.0f;
                        // TO IMPROVE - getting the texture for each face
                        if (tex == GRASS_TOP_TEXTURE) {
                            grass_flag = 2.0f;
                        } else if (tex == GRASS_SIDE_TEXTURE) {
                            grass_flag = 1.0f;
                        }

                        // Once for each vertex
                        grassflags.push_back(grass_flag);
                        grassflags.push_back(grass_flag);
                        grassflags.push_back(grass_flag);
                        grassflags.push_back(grass_flag);

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

   	chunk_mesh = std::make_unique<Mesh>(vertices, indices, texcoords, grassflags);

}

void Chunk::render() {
	chunk_mesh->draw();
}
