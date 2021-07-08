/**
 * All of the chunks and other entities in a world.
 */

#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <world/blocks.hpp>
#include <world/chunk.hpp>

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
	static const int
		WORLD_HEIGHT = 4, // in chunks
		WORLD_WIDTH  = 8; // in chunks
	std::unordered_map<glm::vec3, Chunk*, Vec3KeyUtils, Vec3KeyUtils> loaded_chunks;
	Chunk* get_chunk_containing_coords(int x, int y, int z);

public:
	World();
	Chunk* get_chunk_at(int x, int y, int z);
	bool get_block_at(int x, int y, int z, BlockType& block);
	bool set_block_at(int x, int y, int z, BlockType  block);
	void render();

};
