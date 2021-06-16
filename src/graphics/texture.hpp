/**
 * Provides utilities for loading textures.
 */

#pragma once

#include <vector>

enum Texture {
	GRASS_TOP = 240,
	INVALID   = 19,
};

namespace tex {

// Load the texture atlas
void load();

// Unload it
void unload();

// Get the texture atlas vertices.
std::vector<float> get_vertices(Texture tex);

}
