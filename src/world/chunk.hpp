/**
 * Chunk - a chunk class that represents 16x16x16 blocks.
 */

#pragma once

#include <array>
#include <memory>

#include <graphics/mesh.hpp>
#include <world/blocks.hpp>
#include <world/gen/generator.hpp>

const int CHUNK_SIZE = 16;

// To detect blocks in update_mesh
class World;

class CachedChunk;

class Chunk {

private:
	// All of the blocks in the mesh
	std::array<BlockType, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> blocks;
	// The mesh of all of the blocks
	std::unique_ptr<Mesh> chunk_mesh;
	bool block_borders_air(int i, int j, int k, Side side, World* world);

public:
	Chunk() = default;
	// Read this chunk from a cache
	Chunk(CachedChunk*);
	Chunk(int x, int y, int z, WorldGenerator generator);
	// only do this when the blocks in the chunk are changed
	// cx, cy, cz are chunk coordinates
	void update_mesh(World* world, int cx, int cy, int cz);
	BlockType& at(int x, int y, int z);
	void render();

friend class CachedChunk;

};
