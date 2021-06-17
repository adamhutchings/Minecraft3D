#include "keyboard.hpp"

namespace {

enum MovementMode {

	UP         = 1 ,
	DOWN       = 2 ,
	LEFT       = 4 ,
	RIGHT      = 8 ,
	FORWARD    = 16,
	BACKWARD   = 32,

};

// Can move multiple directions at once.
MovementMode movement = 0;

void keyboard_callback(GLFWwindow* ignore, int key, int ignore2, int action, int mods) {

	MovementMode flag_to_change;

	switch (key) {
		case GLFW_KEY_A:
			flag_to_change = LEFT;
			break;
		case GLFW_KEY_S:
			flag_to_change = BACKWARD;
			break;
		case GLFW_KEY_D:
			flag_to_change = RIGHT;
			break;
		case GLFW_KEY_W:
			flag_to_change = FORWARD;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			flag_to_change = DOWN;
			break;
		case GLFW_KEY_SPACE:
			flag_to_change = UP;
			break;
		default:
			flag_to_change = 0;
	}

	// We only care about press and release, no repeat
	int start_movement;
	if (action == GLFW_PRESS) {
		start_movement = 1;
	} else if (action == GLFW_RELEASE) {
		start_movement = 0;
	} else {
		return;
	}

	if (start_movement) {
		movement |= flag_to_change; // set to true
	} else {
		movement &= ~flag_to_change; // set to false
	}

}

}

void initialize_keyboard_callback(GLFWwindow* wn) {
	glfwSetKeyCallback(wn, keyboard_callback);
}