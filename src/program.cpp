#include <iostream>
#include <vector>

#include "program.hpp"
#include "gl_util.hpp"

GLuint compile_shader(const std::string &src, GLenum type) {
	GLuint shader = glCreateShader(type);
	if(!shader) {
		throw std::runtime_error("Failed to create shader id");
	}

	const char *src_str = src.c_str();
	glShaderSource(shader, 1, &src_str, NULL);
	glCompileShader(shader);
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE) {
		GLint max_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> errlog(max_length);
		glGetShaderInfoLog(shader, max_length, &max_length, &errlog[0]);

		std::cerr << "Compilation error:\n" << &errlog[0] << std::endl;
		glDeleteShader(shader);
		throw std::runtime_error("Failed to compile shader");
	}

	return shader;
}

GLProgram::GLProgram(const std::string &vert_src,
		const std::string &frag_src) : pid(0) {
	GLuint vsh, fsh;

	vsh = compile_shader(vert_src, GL_VERTEX_SHADER);
	if(frag_src != "") {
		fsh = compile_shader(frag_src, GL_FRAGMENT_SHADER);
	}

	this->pid = glCreateProgram();

	if(!this->pid) {
		throw std::runtime_error("Failed to create program id");
	}

	glAttachShader(this->pid, vsh);
	if(fsh) {
		glAttachShader(this->pid, fsh);
	}

	glBindFragDataLocation(this->pid, 0, "colour");
	glLinkProgram(this->pid);

	GLint is_linked = 0;
	glGetProgramiv(this->pid, GL_LINK_STATUS, &is_linked);
	if(is_linked == GL_FALSE) {
		GLint max_length = 0;
		glGetProgramiv(this->pid, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> errlog(max_length);
		glGetProgramInfoLog(this->pid, max_length, &max_length, &errlog[0]);

		std::cerr << "Link error:\n" << &errlog[0] << std::endl;
		glDeleteShader(this->pid);
		this->pid = 0;
		throw std::runtime_error("Failed to link program");
	}

	if(vsh) glDeleteShader(vsh);
	if(fsh) glDeleteShader(fsh);

	check_gl_error("linking program", 3500);
}

GLProgram::~GLProgram() {
	if(this->pid) glDeleteProgram(this->pid);
}

GLuint GLProgram::program_id() {
	return this->pid;
}

void GLProgram::bind() {
	glUseProgram(this->pid);
}

