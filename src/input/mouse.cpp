#include "mouse.hpp"

#include <application.hpp>
#include <world/blocks.hpp>

#include <iostream>

namespace {

auto sensitivity = 0.1f;

// The current mouse location
double cx = 250.0, cy = 250.0;

void mouse_move_callback(GLFWwindow* wn, double x, double y) {

	int xdiff = x - cx, ydiff = y - cy;

	global_app->player->camera.yaw(xdiff * sensitivity, ViewMode::ADJUST);
	global_app->player->camera.pitch(ydiff * sensitivity, ViewMode::ADJUST);

	cx = x, cy = y;

}

void mouse_click_callback(GLFWwindow* wn, int button, int action, int mods) {

	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {

		int x, y, z;
		if ( global_app->player->get_block_break_location(x, y, z) ) {
			global_app->world->set_block_at(x, y, z, BlockType::AIR_BLOCK);
		}

	}

}

}

namespace input {

void initialize_mouse_callbacks(GLFWwindow* wn) {
	glfwGetCursorPos(wn, &cx, &cy);
	glfwSetInputMode(wn, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(wn, mouse_move_callback);
	glfwSetMouseButtonCallback(wn, mouse_click_callback);
}

}
