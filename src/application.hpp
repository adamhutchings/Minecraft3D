/**
 * The controlling class of the game.
 */

#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <world/chunk.hpp>

class Application {

private:
	GLFWwindow* wn;
	void init_context(int major, int minor);
	bool closing = false;
	std::unique_ptr<Chunk> chunk;

public:
	Application();
	void mainloop();
	~Application();
	void close(); // set to close

};

extern Application* global_app;