#include "world.hpp"

World::World() {

	for (int x = 0; x < WORLD_WIDTH; ++x) {
		for (int y = 0; y < WORLD_HEIGHT; ++y) {
			for (int z = 0; z < WORLD_WIDTH; ++z) {
				at(x, y, z) = Chunk(); // TODO
			}
		}
	}

}

Chunk& World::at(int x, int y, int z) {
	return chunks.at(y * WORLD_WIDTH * WORLD_WIDTH + z * WORLD_HEIGHT + x);
}
