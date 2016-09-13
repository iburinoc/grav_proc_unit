#pragma once

#include <string>

#include "gpu/gl.hpp"

class GLProgram {
	GLuint pid;
public:
	GLProgram(const std::string &vert_src,
		const std::string &frag_src=std::string(""));
	virtual ~GLProgram();

	GLuint program_id();
	void bind();
};

