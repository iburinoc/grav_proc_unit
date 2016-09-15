#pragma once

#include <unordered_map>
#include <initializer_list>
#include <string>

#include "gpu/gl.hpp"
#include "gpu/glm.hpp"

#include "program.hpp"

class Uniforms {
	std::unordered_map<std::string, GLint> locations;

	void add_name(GLProgram &prog, const std::string &name);
	GLint get_location(std::string &name);
public:
	Uniforms(GLProgram &prog, std::initializer_list<std::string> names);

	void set_uniform(std::string name, float value);
	void set_uniform(std::string name, mat4 &value);
};

