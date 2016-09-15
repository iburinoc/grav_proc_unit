#include <unordered_map>
#include <stdexcept>
#include <iostream>

#include "gpu/gl.hpp"
#include "gpu/glm.hpp"

#include "grav_proc_unit.hpp"
#include "program.hpp"
#include "shaders.hpp"
#include "gl_util.hpp"

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
		prog(GLProgram(STAR_VERT_SHADER, STAR_FRAG_SHADER)),
		buf(this->prog, 1),
		uniforms(this->prog, {"proj", "view", }),
		camera_pos(vec3(0, 0, -5)),
		camera_orient(quat(1, 0, 0, 0)) {

	callback_map[window] = this;
	glfwSetKeyCallback(window, static_key_callback);
	glfwSetFramebufferSizeCallback(window, static_resize_callback);

	this->init_projection();
}

void GravProcUnit::main_loop() {
	while(!glfwWindowShouldClose(window)) {
		this->update();
		// this->backend->update
		this->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void GravProcUnit::update() {
	double prev_time = this->current_time;
	this->current_time = glfwGetTime();

	double dt = this->current_time - prev_time;

	this->update_camera(dt);
}

void GravProcUnit::update_camera(double dt) {
	vec3 ds = vec3(0, 0, 0);
	vec3 om = vec3(0, 0, 0);

#define CHECK_KEY(x) (glfwGetKey(this->window, GLFW_KEY_ ## x) == GLFW_PRESS)
	if(CHECK_KEY(Q)) ds += vec3( 0,  0, -1);
	if(CHECK_KEY(E)) ds += vec3( 0,  0,  1);
	if(CHECK_KEY(S)) ds += vec3( 0, -1,  0);
	if(CHECK_KEY(W)) ds += vec3( 0,  1,  0);
	if(CHECK_KEY(A)) ds += vec3( 1,  0,  0);
	if(CHECK_KEY(D)) ds += vec3(-1,  0,  0);

	if(dot(ds, ds) > 1e-9) ds /= length(ds);

	if(CHECK_KEY(U)) om += vec3( 0,  0, -1);
	if(CHECK_KEY(O)) om += vec3( 0,  0,  1);
	if(CHECK_KEY(J)) om += vec3( 0,  1,  0);
	if(CHECK_KEY(L)) om += vec3( 0, -1,  0);
	if(CHECK_KEY(I)) om += vec3(-1,  0,  0);
	if(CHECK_KEY(K)) om += vec3( 1,  0,  0);
#undef CHECK_KEY

	const float MOVE_SPEED = 5;
	const float ROT_SPEED = 1;

	ds *= MOVE_SPEED * dt;

	quat rot(1, 0, 0, 0);
	if(dot(om, om) > 1e-9) {
		rot = glm::angleAxis((float) (ROT_SPEED * dt), om);
	}

	this->camera_pos += this->camera_orient * ds;
	this->camera_orient = glm::normalize(this->camera_orient * rot);

	std::cout << glm::to_string(this->camera_pos) << "," << glm::to_string(this->camera_orient) << std::endl;
}

void GravProcUnit::init_projection() {
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	this->proj = glm::infinitePerspective(90.f, width / float(height),
			0.1f);

	this->pix_ratio = 1.0 / width;
}

void GravProcUnit::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(1.0f, 0.5f, 0.0f, 1.0f);

	this->prog.bind();
	this->bind_uniforms();
	this->buf.draw();
}

void GravProcUnit::bind_uniforms() {
	mat4 view(1.0f);

	view = glm::translate(-camera_pos) * view;
	view = glm::mat4_cast(glm::inverse(camera_orient)) * view;
	view = glm::mat4_cast(glm::angleAxis((float)M_PI, vec3(0, 1, 0))) * view;

	glUniformMatrix4fv(this->view_loc, 1, false, value_ptr(view));

	this->uniforms.set_uniform("proj", this->proj);
	this->uniforms.set_uniform("view", view);
}

void GravProcUnit::key_callback(int key, int code, int action, int mods) {
	if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(this->window, 1);
	}
}

void GravProcUnit::resize_callback(int width, int height) {
	this->init_projection();
}

