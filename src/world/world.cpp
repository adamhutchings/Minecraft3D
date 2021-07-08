#include "world.hpp"

#include <cmath>

#include <world/gen/generator.hpp>

World::World() {

	WorldGenerator generator{};

	for (int x = 0; x < WORLD_WIDTH; ++x) {
		for (int y = 0; y < WORLD_HEIGHT; ++y) {
			for (int z = 0; z < WORLD_WIDTH; ++z) {
				loaded_chunks[glm::vec3(x, y, z)] = new Chunk(x, y, z, generator);
			}
		}
	}

	for (int x = 0; x < WORLD_WIDTH; ++x) {
		for (int y = 0; y < WORLD_HEIGHT; ++y) {
			for (int z = 0; z < WORLD_WIDTH; ++z) {
				get_chunk_at(x, y, z)->update_mesh(this);
			}
		}
	}

}

Chunk* World::get_chunk_containing_coords(int x, int y, int z) {

	return get_chunk_at(

		std::floor( (float) x / CHUNK_SIZE ),
		std::floor( (float) y / CHUNK_SIZE ),
		std::floor( (float) z / CHUNK_SIZE )

	);

}

Chunk* World::get_chunk_at(int x, int y, int z) {

	glm::vec3 vec(x, y, z);

	if (loaded_chunks.find(vec) != loaded_chunks.end()) {
		return loaded_chunks[vec];
	}

	return nullptr;

}

bool World::get_block_at(int x, int y, int z, BlockType& block) {

	if ( (x < 0) || (y < 0) || (z < 0) ||
		(x >= WORLD_WIDTH  * CHUNK_SIZE)
	||  (y >= WORLD_HEIGHT * CHUNK_SIZE)
	||  (z >= WORLD_WIDTH  * CHUNK_SIZE)
	) {
		block = AIR_BLOCK;
		return false;
	}

	block = get_chunk_containing_coords(x, y, z) ->at(x % CHUNK_SIZE, y % CHUNK_SIZE, z % CHUNK_SIZE);
	return true;

}

bool World::set_block_at(int x, int y, int z, BlockType block) {

	auto chunk = get_chunk_containing_coords(x, y, z);

	if (chunk != nullptr) {
		chunk->at(x % CHUNK_SIZE, y % CHUNK_SIZE, z % CHUNK_SIZE) = block;
		chunk->update_mesh(this);
	}

	return chunk != nullptr;

}

void World::render() {
	
	for (int x = 0; x < WORLD_WIDTH; ++x) {
		for (int y = 0; y < WORLD_HEIGHT; ++y) {
			for (int z = 0; z < WORLD_WIDTH; ++z) {
				get_chunk_at(x, y, z)->render();
			}
		}
	}

}
