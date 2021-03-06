/**
 * This provides a function to initialize the GLFW mouse callback, so that the
 * camera can respond to mouse input.
 */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace input {

/**
 * This also locks the mouse so it's hidden.
 */
void initialize_mouse_callbacks(GLFWwindow* wn);

}
