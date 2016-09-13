#pragma once

#include "gpu/gl.hpp"
#include "gpu/glm.hpp"

#include "program.hpp"
#include "buffer.hpp"

class GravProcUnit {
	GLFWwindow* window;
	GLProgram prog;
	StarBuffer buf;

	mat4 proj;

	vec3 camera_pos;
	quat camera_orient;

	double current_time;
public:
	GravProcUnit(GLFWwindow* window);

	void main_loop();

	void key_callback(int key, int code, int action, int mods);
	void resize_callback(int width, int height);

private:
	void update();
	void update_camera(double dt);

	void init_projection();
	void render();

	GLint proj_loc, view_loc;
	void init_uniform_locations();
	void bind_uniforms();
};

