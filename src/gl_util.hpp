#pragma once

#include <string>

#include "gpu/gl.hpp"

GLFWwindow *init_gl();
void end_gl(GLFWwindow *window);

void check_gl_error(const char *err_str, int code);

