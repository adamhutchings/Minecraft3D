#include "world.hpp"

#include <world/gen/generator.hpp>

World::World() {

	WorldGenerator generator{};

	for (int x = 0; x < WORLD_WIDTH; ++x) {
		for (int y = 0; y < WORLD_HEIGHT; ++y) {
			for (int z = 0; z < WORLD_WIDTH; ++z) {
				chunks.at(y * WORLD_WIDTH * WORLD_WIDTH + x * WORLD_WIDTH + z) = new Chunk(x, y, z, generator);
			}
		}
	}

	for (int x = 0; x < WORLD_WIDTH; ++x) {
		for (int y = 0; y < WORLD_HEIGHT; ++y) {
			for (int z = 0; z < WORLD_WIDTH; ++z) {
				chunk_at(x, y, z)->update_mesh(this);
			}
		}
	}

}

Chunk* World::chunk_at(int x, int y, int z) {

	if (
		x < 0 || x >= WORLD_WIDTH  ||
		y < 0 || y >= WORLD_HEIGHT ||
		z < 0 || z >= WORLD_WIDTH
	) return nullptr;

	return chunks.at(y * WORLD_WIDTH * WORLD_WIDTH + x * WORLD_WIDTH + z);

}

BlockType World::block_at(int x, int y, int z) {
	if ( (x < 0) || (y < 0) || (z < 0) ) return AIR_BLOCK;
	if (
		(x >= WORLD_WIDTH  * CHUNK_SIZE)
	||  (y >= WORLD_HEIGHT * CHUNK_SIZE)
	||  (z >= WORLD_WIDTH  * CHUNK_SIZE)
	) return AIR_BLOCK;
	return  chunk_at(x / CHUNK_SIZE, y / CHUNK_SIZE, z / CHUNK_SIZE)
	        ->    at(x % CHUNK_SIZE, y % CHUNK_SIZE, z % CHUNK_SIZE);
}

void World::set_block_at(int x, int y, int z, BlockType block) {
	// TODO TODO TODO error checkings
	  chunk_at(x / CHUNK_SIZE, y / CHUNK_SIZE, z / CHUNK_SIZE)
	  ->    at(x % CHUNK_SIZE, y % CHUNK_SIZE, z % CHUNK_SIZE)
	 = block;

	chunk_at(x / CHUNK_SIZE, y / CHUNK_SIZE, z / CHUNK_SIZE)->update_mesh(this);

}

void World::render() {
	
	for (int x = 0; x < WORLD_WIDTH; ++x) {
		for (int y = 0; y < WORLD_HEIGHT; ++y) {
			for (int z = 0; z < WORLD_WIDTH; ++z) {
				chunk_at(x, y, z)->render();
			}
		}
	}

}
