#pragma once

#include "gpu/gl.hpp"
#include "gpu/glm.hpp"

#include "program.hpp"

class GravProcUnit {
	GLFWwindow* window;
	GLProgram prog;
	StarBuffer buf;

	mat4 proj;

	vec3 camera_pos;
	quat camera_orient;
public:
	GravProcUnit(GLFWwindow* window);

	void main_loop();

	void init_projection();
	void render();

	void key_callback(int key, int code, int action, int mods);
	void resize_callback(int width, int height);
};

