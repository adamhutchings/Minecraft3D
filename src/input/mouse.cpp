#include "mouse.hpp"

#include <application.hpp>

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
