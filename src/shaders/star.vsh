R"(
#version 330 core

in vec3 position;
in float size;

uniform mat4 view;
uniform mat4 proj;

void main() {
	//gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
	gl_PointSize = size;
	gl_Position = proj * view * vec4(position, 1);
}
)"