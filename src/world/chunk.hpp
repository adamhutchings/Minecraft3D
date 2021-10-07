/**
 * Chunk - a chunk class that represents 16x16x16 blocks.
 */

#pragma once

#include <array>
#include <memory>

#include <glm/glm.hpp>

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
	bool trees_generated_already = false;

public:
	// Not needed to be cached - let's actually not update the mesh whenever a
	// block is placed, but only if we need to when the frame is refreshed.
	bool updated_since_last_frame = false;
	Chunk() = default;
	// Read this chunk from a cache
	Chunk(CachedChunk*);
	Chunk(int x, int y, int z, WorldGenerator generator, World* world);
	// only do this when the blocks in the chunk are changed
	// cx, cy, cz are chunk coordinates
	void update_mesh(World* world, int cx, int cy, int cz);
	BlockType& at(int x, int y, int z);
	void render();
	// Generate all trees. Fail if trees have been generated already.
	void generate_trees(World* world, glm::vec3 chunk_loc);

friend class CachedChunk;
friend class World;

};
