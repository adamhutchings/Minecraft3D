#version 410 core

in vec3 vertexColor;

out vec4 fragColor;

void main() {
	fragColor = vertexColor;
}
