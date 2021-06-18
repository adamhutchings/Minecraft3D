#include "world.hpp"

World::World() {

	for (int x = 0; x < WORLD_WIDTH; ++x) {
		for (int y = 0; y < WORLD_HEIGHT; ++y) {
			for (int z = 0; z < WORLD_WIDTH; ++z) {
				at(x, y, z) = Chunk(x, y, z);
			}
		}
	}

}

Chunk& World::at(int x, int y, int z) {
	return chunks.at(y * WORLD_WIDTH * WORLD_WIDTH + x * WORLD_WIDTH + z);
}

BlockType& World::block_at(int x, int y, int z) {
	return at(x / CHUNK_SIZE, y / CHUNK_SIZE, z / CHUNK_SIZE)
	      .at(x % CHUNK_SIZE, y % CHUNK_SIZE, z % CHUNK_SIZE);
}

void World::render() {
	
	for (int x = 0; x < WORLD_WIDTH; ++x) {
		for (int y = 0; y < WORLD_HEIGHT; ++y) {
			for (int z = 0; z < WORLD_WIDTH; ++z) {
				at(x, y, z).render();
			}
		}
	}

}
