#version 410 core

in vec2 texCoord;

uniform sampler2D sampl;

out vec4 fragColor;

void main() {
	fragColor = texture(sampl, texCoord);
}
