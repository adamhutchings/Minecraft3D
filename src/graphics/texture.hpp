/**
 * Provides utilities for loading textures.
 */

#pragma once

#include <vector>

enum Texture {
	DIRT_TEXTURE                 = 242,
	GRASS_TOP_TEXTURE            = 240,
	STONE_TEXTURE                = 241,
	GRASS_SIDE_TEXTURE           = 243,
	INVALID_TEXTURE              = 19,
	OAK_WOOD_INNER_TEXTURE       = 229,
	OAK_WOOD_SIDE_TEXTURE        = 228,
	OAK_LEAVES_TEXTURE           = 197,
};

namespace tex {

// Load the texture atlas
void load();

// Unload it
void unload();

// Get the texture atlas vertices.
std::vector<float> get_vertices(Texture tex);

}
