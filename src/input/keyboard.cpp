#include "keyboard.hpp"

#include <application.hpp>
#include <graphics/camera.hpp>

namespace {

namespace movement_mode {

int NONE       = 0 ,
	UP         = 1 ,
	DOWN       = 2 ,
	LEFT       = 4 ,
	RIGHT      = 8 ,
	FORWARD    = 16,
	BACKWARD   = 32
;

}

// Can move multiple directions at once.
int movement = movement_mode::NONE;

// How much to move.
float movement_speed = 0.3f;

void keyboard_callback(GLFWwindow* ignore, int key, int ignore2, int action, int mods) {

	int flag_to_change = movement_mode::NONE;

	switch (key) {
		case GLFW_KEY_A:
			flag_to_change = movement_mode::LEFT;
			break;
		case GLFW_KEY_S:
			flag_to_change = movement_mode::BACKWARD;
			break;
		case GLFW_KEY_D:
			flag_to_change = movement_mode::RIGHT;
			break;
		case GLFW_KEY_W:
			flag_to_change = movement_mode::FORWARD;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			flag_to_change = movement_mode::DOWN;
			break;
		case GLFW_KEY_SPACE:
			flag_to_change = movement_mode::UP;
			break;
		case GLFW_KEY_ESCAPE:
			global_app->close();
		default:;
	}

	if (flag_to_change == movement_mode::NONE) return;

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

namespace input {

void initialize_keyboard_callback(GLFWwindow* wn) {
	glfwSetKeyCallback(wn, keyboard_callback);
}

void do_movement() {

	if (movement & movement_mode::UP)
		global_camera.position(0, movement_speed, 0, ViewMode::ADJUST);
	if (movement & movement_mode::DOWN)
		global_camera.position(0, -movement_speed, 0, ViewMode::ADJUST);
	if (movement & movement_mode::LEFT)
		global_camera.move(movement_speed, -90.0f);
	if (movement & movement_mode::RIGHT)
		global_camera.move(movement_speed,  90.0f);
	if (movement & movement_mode::FORWARD)
		global_camera.move(movement_speed,  0.0f);
	if (movement & movement_mode::BACKWARD)
		global_camera.move(movement_speed,  180.0f);

}

}
