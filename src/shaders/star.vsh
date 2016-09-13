R"(
#version 330 core

in vec3 position;
in float size;

uniform mat4 view;
uniform mat4 proj;

uniform float pix_ratio;

void main() {
	gl_PointSize = size;
	gl_Position = proj * view * vec4(position, 1);
}
)"
