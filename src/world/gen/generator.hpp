#pragma once

#include <world/blocks.hpp>
#include <world/gen/PerlinNoise.hpp>

class WorldGenerator {

private:
	siv::PerlinNoise noise;

public:
	WorldGenerator();

	// Use this in chunk gen.
	BlockType get_block_at(int x, int y, int z);

};
