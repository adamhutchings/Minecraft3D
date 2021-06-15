/**
 * A wrapper around a complete shader program.
 */

#pragma once

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <memory>
#include <string>

class Shader {

private:
    // The OpenGL ID of this shader.
    unsigned program_id;
    // Create a single vertex or fragment shader.
    int create_subshader(const char* code, int type);
    // Finalize the shader.
    void link(int vert, int frag);
    // Map of shader name to int ID.
    std::map<std::string, int> uniforms;

public:
    // Take in the file paths.
    Shader(const char* vertex_path, const char* fragment_path);
    void bind(); // set as active shader
    void unbind();
    void destroy();
    void register_uniform(std::string name);
    void set_uniform_value(std::string name, glm::mat4 value);

};

extern std::unique_ptr<Shader> GAME_SHADER; // a basic shader

inline void shader_init() {
    GAME_SHADER = std::make_unique<Shader>("src/shader/game_vert.glsl", "src/shader/game_frag.glsl");
}

inline void shader_destroy() {
    GAME_SHADER->destroy();
}
