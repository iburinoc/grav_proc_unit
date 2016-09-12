#pragma once

#include "gpu/gl.h"

class StarBuffer {
	GLuint vao;
	GLuint vbo;

	int count;
public:
	StarBuffer(int count);
	virtual ~StarBuffer()

	void draw();
};

