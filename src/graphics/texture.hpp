/**
 * Provides utilities for loading textures.
 */

#pragma once

#include <vector>

enum Texture {
	GRASS_TOP_TEXTURE            = 240,
	INVALID_TEXTURE              = 19,
};

namespace tex {

// Load the texture atlas
void load();

// Unload it
void unload();

// Get the texture atlas vertices.
std::vector<float> get_vertices(Texture tex);

}
