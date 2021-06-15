#version 410 core

layout(location=0) in vec3   inVertexCoord;
layout(location=1) in vec3   inVertexColor;

out vec3 vertexColor;

void main() {
    gl_Position = vec4(inVertexCoord, 1.0);
    vertexColor = inVertexColor;
}
