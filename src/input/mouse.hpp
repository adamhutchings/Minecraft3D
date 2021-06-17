/**
 * This provides a function to initialize the GLFW mouse callback, so that the
 * camera can respond to mouse input.
 */

#pragma once

namespace input {

/**
 * This also locks the mouse so it's hidden.
 */
void initialize_mouse_callback(void* wn);

}
