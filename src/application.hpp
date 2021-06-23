/**
 * The controlling class of the game.
 */

#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <player/player.hpp>
#include <world/world.hpp>

class Application {

private:

	GLFWwindow* wn;

	// Request an OpenGL context of major.minor .
	// e.g. init_context(3, 3) gives an OpenGL 3.3 context.
	void init_context(int major, int minor);

	// Whether the application has been set elsewhere
	// in the game to close after the current tick.
	bool closing = false;

public:
	Application();
	// Run the game.
	void mainloop();
	~Application();
	void close(); // set to close
	std::unique_ptr<World> world;
	std::unique_ptr<Player> player;

};

// Access objects like the global camera through here.
extern Application* global_app;
