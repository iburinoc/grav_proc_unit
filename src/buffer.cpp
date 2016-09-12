#include "gpu/gl.hpp"

#include "buffer.hpp"
#include "program.hpp"

StarBuffer::StarBuffer(GLProgram &prog, int count) :
		count(count) {
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	float data[4] = {0, 0, 0, 5};

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	GLint pos_loc = glGetAttribLocation(prog.program_id(), "position");
	GLint size_loc = glGetAttribLocation(prog.program_id(), "size");

	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 4*sizeof(GLfloat),
		(GLvoid*) 0);
	glVertexAttribPointer(size_loc, 1, GL_FLOAT, false, 4*sizeof(GLfloat),
		(GLvoid*) (3*sizeof(GLfloat)));

	glBindVertexArray(0);
}

StarBuffer::~StarBuffer() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
}

void StarBuffer::draw() {
	glBindVertexArray(this->vao);
	glDrawArrays(GL_POINTS, 0, 1);
}

