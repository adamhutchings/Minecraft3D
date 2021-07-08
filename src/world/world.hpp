/**
 * All of the chunks and other entities in a world.
 */

#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <world/blocks.hpp>
#include <world/chunk.hpp>
#include <world/save/chunk_cache.hpp>

struct Vec3KeyUtils {

	size_t operator() (const glm::vec3& k) const {
        return std::hash<int>()(k.x) ^ (2 * std::hash<int>()(k.y)) ^ (4 * std::hash<int>()(k.z));
    }

	bool operator()(const glm::vec3& a, const glm::vec3& b) const {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

class World {

private:
	WorldGenerator generator;
	static const int
		WORLD_HEIGHT = 4, // in chunks
		WORLD_WIDTH  = 8; // in chunks
	std::unordered_map<glm::vec3, Chunk*,      Vec3KeyUtils, Vec3KeyUtils> loaded_chunks;
	std::unordered_map<glm::vec3, CachedChunk, Vec3KeyUtils, Vec3KeyUtils> unloaded_chunks;
	Chunk* get_chunk_containing_coords(int x, int y, int z);
	// Take a chunk out of the loaded chunks and save it into
	// the unloaded chunks. Return whether the operation
	// succeeded.
	bool unload_chunk(int x, int y, int z);
	// Either take a chunk out of the unloaded chunks and load
	// its data into the world or generate a fresh new chunk.
	// Return whether the op succeeded.
	bool   load_chunk(int x, int y, int z);

public:
	World();
	Chunk* get_chunk_at(int x, int y, int z);
	bool get_block_at(int x, int y, int z, BlockType& block);
	bool set_block_at(int x, int y, int z, BlockType  block);
	void render();

};
