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
		const std::string &frag_src) : pid(0), vsh(0), fsh(0) {
	this->vsh = compile_shader(vert_src, GL_VERTEX_SHADER);
	if(frag_src != "") {
		this->fsh = compile_shader(frag_src, GL_FRAGMENT_SHADER);
	}

	this->pid = glCreateProgram();

	if(!this->pid) {
		throw std::runtime_error("Failed to create program id");
	}

	glAttachShader(this->pid, this->vsh);
	if(this->fsh) {
		glAttachShader(this->pid, this->fsh);
	}

	glLinkProgram(this->pid);

	check_gl_error("linking program", 3500);
}

GLProgram::~GLProgram() {
	if(this->pid) glDeleteProgram(this->pid);
	if(this->vsh) glDeleteShader(this->vsh);
	if(this->fsh) glDeleteShader(this->fsh);
}

GLuint GLProgram::program_id() {
	return this->pid;
}

void GLProgram::bind() {
	glUseProgram(this->pid);
}

