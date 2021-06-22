/**
 * The player, who can move, look, break blocks, etc.
 */

#pragma once

#include <graphics/camera.hpp>

class Player {

public:
	Camera camera;
	Player(); // initialize the camera
	// If the player clicks, what block gets broken?
	bool get_block_break_location(int& x, int& y, int& z);

};
