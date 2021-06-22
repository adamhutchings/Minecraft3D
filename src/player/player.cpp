#include "player.hpp"

Player::Player()
: camera() {
	// Nothing needed yet
}

bool Player::get_block_break_location(int& x, int& y, int &z) {
	return camera.get_block_break_location(x, y, z);
}
