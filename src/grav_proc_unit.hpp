#pragma once

#include <memory>

#include "gpu/gl.hpp"
#include "gpu/glm.hpp"

#include "program.hpp"
#include "buffer.hpp"
#include "backend.hpp"
#include "uniforms.hpp"

class GravProcUnit {
	GLFWwindow* window;
	GLProgram prog;
	std::shared_ptr<Backend> backend;
	StarBuffer buf;
	Uniforms uniforms;

	mat4 proj;
	float pix_ratio;

	float distance;
	quat camera_orient;

	double current_time;
public:
	GravProcUnit(GLFWwindow* window, std::unique_ptr<Backend> backend);

	void main_loop();

	void key_callback(int key, int code, int action, int mods);
	void resize_callback(int width, int height);

private:
	void update();
	void update_camera(double dt);

	void init_projection();
	void render();

	GLint view_loc, viewinv_loc;
	void init_uniform_locations();
	void bind_uniforms();
};

