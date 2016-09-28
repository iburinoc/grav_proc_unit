#pragma once

#include <vector>

#include "gpu/gl.hpp"
#include "gpu/glm.hpp"

#include "program.hpp"

class StarBuffer {
	GLuint vao;
	GLuint pvbo, svbo, ebo;

	int count;
public:
	StarBuffer(GLProgram &prog, int count);
	virtual ~StarBuffer();

	void set_vertices(std::vector<vec3> &data, std::vector<GLfloat> &size);

	void draw();
};

