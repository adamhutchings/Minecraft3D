#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <graphics/camera.hpp>
#include <graphics/mesh.hpp>
#include <graphics/shader.hpp>
#include <graphics/texture.hpp>

#include <world/chunk.hpp>

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
	GAME_SHADER->bind();

	tex::load();

	GAME_SHADER->register_uniform("game_matrix");

	Chunk chunk{};

	global_camera.pitch(30.0f, ViewMode::SET);
	global_camera.position(2, 2, 2, ViewMode::SET);
	global_camera.yaw(210.0f, ViewMode::SET);

	while (!glfwWindowShouldClose(wn)) {
		// Update camera position
		GAME_SHADER->set_uniform_value("game_matrix", global_camera.get_transform_matrix());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		chunk.render();
		glfwSwapBuffers(wn);
		glfwPollEvents();
	}

	tex::unload();
	shader_destroy();
	glfwDestroyWindow(wn);
	glfwTerminate();

}
