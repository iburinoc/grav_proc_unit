#include <iostream>
#include <vector>
#include <stdexcept>

#include "gpu/gl.hpp"

#include "options.hpp"
#include "gl_util.hpp"

void error_callback(int error, const char *desc) {
	std::cout << "GL Error: (" << error << ") " << desc << std::endl;
}

GLFWwindow *init_gl() {
	if(!glfwInit()) {
		// Failed to initialize
		std::cerr << "Failed to initialize OpenGL" << std::endl;
		return NULL;
	}

	glfwSetErrorCallback(error_callback);	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(
		options.dimensions.x,
		options.dimensions.y,
		"Gravity Processing Unit",
		NULL,
		NULL);

	if(!window) {
		return NULL;
	}

	glfwMakeContextCurrent(window);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	glfwSwapInterval(1);

	glewInit();

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	return window;
}

void end_gl(GLFWwindow *window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

