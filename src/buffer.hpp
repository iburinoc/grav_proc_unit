#pragma once

#include <memory>
#include <vector>

#include "gpu/gl.hpp"
#include "gpu/glm.hpp"

#include "program.hpp"
#include "backend.hpp"

class StarBuffer {
	GLuint vao;
	GLuint pvbo, svbo, ebo;

	const std::shared_ptr<Backend> bk;

	int count;
public:
	StarBuffer(GLProgram &prog, const std::shared_ptr<Backend> &bk);
	virtual ~StarBuffer();

	void draw();

	GLuint get_pvbo() const;
	GLuint get_svbo() const;

	void set_count(int c);
};

