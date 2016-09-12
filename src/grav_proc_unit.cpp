#include <unordered_map>
#include <stdexcept>

#include "gpu/gl.hpp"

#include "grav_proc_unit.hpp"
#include "program.hpp"
#include "shaders.hpp"

static std::unordered_map<GLFWwindow *, GravProcUnit *> callback_map;

static void static_key_callback(GLFWwindow *window, int key, int code,
		int action, int mods) {
	if(callback_map.count(window) == 0) {
		throw std::runtime_error("Instance not found for callback");
	}
	callback_map[window]->key_callback(key, code, action, mods);
}

static void static_resize_callback(GLFWwindow *window, int width, int height) {
	if(callback_map.count(window) == 0) {
		throw std::runtime_error("Instance not found for callback");
	}
	callback_map[window]->resize_callback(width, height);
}


GravProcUnit::GravProcUnit(GLFWwindow* window) :
		window(window),
		prog(GLProgram(STAR_VERT_SHADER)),
		buf(this->prog, 1),
		camera_pos(vec3(0, 0, -1)),
		camera_orient(quat(1, 0, 0, 0)) {

	callback_map[window] = this;
	glfwSetKeyCallback(window, static_key_callback);
	glfwSetFramebufferSizeCallback(window, static_resize_callback);

	this->init_projection();
}

void GravProcUnit::main_loop() {
	while(!glfwWindowShouldClose(window)) {
		// this->backend->update
		this->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void GravProcUnit::init_projection() {
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	this->proj = glm::infinitePerspective(90.f, width / float(height),
			0.1f);
}

void GravProcUnit::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->prog.bind();
	this->buf.draw();
}

void GravProcUnit::key_callback(int key, int code, int action, int mods) {
	if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(this->window, 1);
	}
}

void GravProcUnit::resize_callback(int width, int height) {
	this->init_projection();
}
