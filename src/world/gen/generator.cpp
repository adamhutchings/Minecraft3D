#include "generator.hpp"

#include <random>

WorldGenerator::WorldGenerator()
: noise(std::random_device{}) {

}

BlockType WorldGenerator::get_block_at(int x, int y, int z) {

	// This is the level of the grass at a given coordinate.
	int cutoff = noise.accumulatedOctaveNoise2D_0_1((float) x / 64, (float) z / 64, 2) * 32 + 32;

	if (y > cutoff)     return AIR_BLOCK;
	if (y == cutoff)    return GRASS_BLOCK;
	if (y > cutoff - 4) return DIRT_BLOCK;
	return STONE_BLOCK;

}
