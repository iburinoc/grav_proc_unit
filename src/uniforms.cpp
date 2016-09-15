#include <unordered_map>
#include <initializer_list>
#include <string>
#include <exception>

#include "gpu/gl.hpp"

#include "uniforms.hpp"
#include "program.hpp"
#include "gl_util.hpp"

Uniforms::Uniforms(GLProgram &prog, std::initializer_list<std::string> names) {
	for(const std::string &name : names) {
		this->add_name(prog, name);
	}
}

void Uniforms::add_name(GLProgram &prog, const std::string &name) {
	GLint loc = glGetUniformLocation(prog.program_id(), name.c_str());
	check_gl_error(("Getting uniform location " + name).c_str(), 4320);
	if(loc == -1) {
		throw std::runtime_error(
			("Failed to load uniform " + name));
	}

	this->locations[name] = loc;
}

GLint Uniforms::get_location(std::string &name) {
	if(this->locations.count(name) == 0) {
		throw std::runtime_error(
			("Uniform name " + name + " not registered"));
	}

	return this->locations[name];
}

void Uniforms::set_uniform(std::string name, float value) {
	GLint loc = this->get_location(name);
	glUniform1f(loc, value);
}

void Uniforms::set_uniform(std::string name, mat4 &value) {
	GLint loc = this->get_location(name);
	glUniformMatrix4fv(loc, 1, false, value_ptr(value));
}

