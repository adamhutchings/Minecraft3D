#include "shader.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#include <glad/glad.h>

namespace {

// Load a whole file as a string.
const GLchar* load_file_as_string(const char* path) {
    // TODO - do this with C++ classes.
    FILE* fp = fopen(path, "r");
    if (!fp) return nullptr;
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    // Yes, I know the memes. Haha. Funny.
    char* code = (char*) malloc(len + 1);
    rewind(fp);
    for (int pos = 0; pos < len; ++pos) {
        code[pos] = getc(fp);
    }
    code[len] = 0;
    return code;
}

}

Shader::Shader(const char* vertex_path, const char* fragment_path) {

    this->program_id = glCreateProgram();
    // TODO - check for error

    auto vertcode = load_file_as_string(vertex_path), fragcode = load_file_as_string(fragment_path);
    int vert = create_subshader(vertcode, GL_VERTEX_SHADER);
    int frag = create_subshader(fragcode, GL_FRAGMENT_SHADER);

    this->link(vert, frag);

    free((void*) vertcode);
    free((void*) fragcode);
    
}

int Shader::create_subshader(const GLchar* code, int type) {

    int id = glCreateShader(type);
    // TODO - check if id == 0

    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);

    GLint compile_status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        // Check for error - if so, ...
        GLchar errorbuf[512];
        glGetShaderInfoLog(id, 512, nullptr, errorbuf);
        std::cerr << std::string(errorbuf) << "\n";
    }

    return id;

}

void Shader::link(int vert, int frag) {
    glAttachShader(this->program_id, vert);
    glAttachShader(this->program_id, frag);
    glLinkProgram (this->program_id);
    // We no longer need these sub-shaders.
    glDetachShader(this->program_id, vert);
    glDetachShader(this->program_id, frag);
}

void Shader::bind() {
    glUseProgram(this->program_id);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::register_uniform(std::string name) {
    int loc = glGetUniformLocation(this->program_id, name.c_str());
    if (loc < 0)
        ; // TODO
    uniforms[name] = loc;
}

void Shader::set_uniform_value(std::string name, glm::mat4 value) {
    glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, &value[0][0]);
}

std::unique_ptr<Shader> GAME_SHADER;
