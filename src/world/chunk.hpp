/**
 * Chunk - a chunk class that represents 16x16x16 blocks.
 */

#pragma once

#include <array>
#include <memory>

#include <graphics/mesh.hpp>
#include <world/blocks.hpp>

class Chunk {

private:
	static const int CHUNK_SIZE = 16;
	// All of the blocks in the mesh
	std::array<BlockType, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> blocks;
	// The mesh of all of the blocks
	std::unique_ptr<Mesh> chunk_mesh;
	bool block_borders_air(int i, int j, int k, Side side);

public:
	Chunk();
	void update_mesh(); // only do this when the blocks in the chunk are changed
	BlockType& at(int x, int y, int z);
	void render();

};
