#include "mouse.hpp"

#include <graphics/camera.hpp>

namespace {

auto sensitivity = 0.1f;

// The current mouse location
double cx = 250.0, cy = 250.0;

void mouse_callback(GLFWwindow* wn, double x, double y) {

	int xdiff = x - cx, ydiff = y - cy;

	global_camera.yaw(xdiff * sensitivity, ViewMode::ADJUST);
	global_camera.pitch(ydiff * sensitivity, ViewMode::ADJUST);

	cx = x, cy = y;

}

}

namespace input {

void initialize_mouse_callback(GLFWwindow* wn) {
	glfwSetInputMode(wn, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(wn, mouse_callback);
}

}
