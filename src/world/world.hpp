/**
 * All of the chunks and other entities in a world.
 */

#pragma once

#include <array>

#include <world/chunk.hpp>

class World {

private:
	static const int
		WORLD_HEIGHT = 4, // in chunks
		WORLD_WIDTH  = 8; // in chunks
	std::array<
		std::array<
			std::array<
				Chunk, WORLD_WIDTH
			>, WORLD_HEIGHT
		>, WORLD_WIDTH
	> chunks;

public:
	World();
	Chunk& at(int x, int y, int z);
	void render();

};
