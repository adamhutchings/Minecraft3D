#include "application.hpp"

#include <chrono>
#include <random>
#include <thread>

#include <graphics/camera.hpp>
#include <graphics/shader.hpp>
#include <graphics/texture.hpp>
#include <input/keyboard.hpp>
#include <input/mouse.hpp>

Application::Application() {

	global_app = this;

	glfwInit(); // TODO - check for error

	// OpenGL 4.1
	this->init_context(4, 1);

	player = std::make_unique<Player>();

	player->camera.pitch(-60.0f, ViewMode::SET);
	player->camera.position(0, 75, 0, ViewMode::SET);
	player->camera.yaw(30.0f, ViewMode::SET);

	// TODO - also check for error
	wn = glfwCreateWindow(500, 500, "Minecraft", nullptr, nullptr);
	glfwMakeContextCurrent(wn);
	gladLoadGL();
	glfwShowWindow(wn);

	input::initialize_mouse_callbacks(wn);
	input::initialize_keyboard_callback(wn);

	// Background sky color
	glClearColor(0.4f, 0.7f, 1.0f, 1.0f);

	shader_init();
	GAME_SHADER->bind();

	tex::load();

	GAME_SHADER->register_uniform("game_matrix");

	world = std::make_unique<World>();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

}

void Application::init_context(int major, int minor) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::mainloop() {

	std::srand(time(NULL));

	while ( (!glfwWindowShouldClose(wn)) && (!closing) ) {

		// We want each frame to last for exactly 1/50th second,
		// so capture the starting time so we can sleep for the
		// needed amount of time at the end of the frame.
		auto start_of_frame = std::chrono::steady_clock::now();

		world->load_unload_one();
		world->generate_trees();

		input::do_movement();

		// Update camera position
		GAME_SHADER->set_uniform_value("game_matrix", player->camera.get_transform_matrix());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		world->render();
		glfwSwapBuffers(wn);
		glfwPollEvents();

		std::this_thread::sleep_until(start_of_frame + std::chrono::milliseconds(20));

	}

}

void Application::close() {
	closing = true;
}

Application::~Application() {
	world->shutdown_update_thread();
	tex::unload();
	shader_destroy();
	glfwDestroyWindow(wn);
	glfwTerminate();
}

Application* global_app;
