#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <graphics/mesh.hpp>
#include <graphics/shader.hpp>
#include <graphics/texture.hpp>

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

	std::vector coords{
		-0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f,
	};

	std::vector texcoords = tex::get_vertices(Texture::GRASS_TOP);

	std::vector indices{
		0, 1, 3, 3, 1, 2,
	};

	Mesh mesh(coords, indices, texcoords);

	while (!glfwWindowShouldClose(wn)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mesh.draw();
		glfwSwapBuffers(wn);
		glfwPollEvents();
	}

	tex::unload();
	shader_destroy();
	glfwDestroyWindow(wn);
	glfwTerminate();

}
