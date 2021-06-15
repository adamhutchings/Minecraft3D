#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <graphics/shader.hpp>

int main() {

	glfwInit(); // TODO - check for error

	// OpenGL 4.1
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// TODO - also check for error
	auto wn = glfwCreateWindow(500, 500, "Minecraft", nullptr, nullptr);
	glfwMakeContextCurrent(wn);
	gladLoadGL();
	glfwShowWindow(wn);

	// Background sky color
	glClearColor(0.4f, 0.7f, 1.0f, 1.0f);

	shader_init();

	while (!glfwWindowShouldClose(wn)) {
		glClear(GL_COLOR_BUFFER_BIT);
		// Render stuff here?
		glfwSwapBuffers(wn);
		glfwPollEvents();
	}

	shader_destroy();
	glfwDestroyWindow(wn);
	glfwTerminate();

}
