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
	return chunks.at(x).at(y).at(z);
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
