/**
 * Provide keyboard callbacks. See mouse.hpp for more detail.
 */

#pragma once

#include <GLFW/glfw3.h>

namespace input {

void initialize_keyboard_callback(GLFWwindow* wn);

void do_movement();

}
