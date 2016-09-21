#include <iostream>
#include <iomanip>
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
		std::cerr << "Failed to initialize GLFW" << std::endl;
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

	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	check_gl_error("GLAD init", 9002);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	check_gl_error("Getting version", 9001);


	glfwSwapInterval(1);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_BLEND);
	check_gl_error("Initializing settings", 2000);

	return window;
}

void end_gl(GLFWwindow *window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void check_gl_error(const char *err_str, int code) {
	GLenum err = glGetError();

	if(err == GL_NO_ERROR) return;

	std::cerr << "ERROR: 0x" << std::setfill('0') << std::setw(4)
		<< std::setbase(16) << err << "," << std::setbase(10) << code
		<< ": " << err_str << std::endl;
	throw std::runtime_error("GL Error");
}

