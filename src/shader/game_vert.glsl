#version 410 core

layout(location=0) in vec3   inVertexCoord;
layout(location=1) in vec2   inTexCoord;

uniform mat4 game_matrix;

out vec2 texCoord;

void main() {
    gl_Position = game_matrix * vec4(inVertexCoord, 1.0);
    texCoord = inTexCoord;
}
