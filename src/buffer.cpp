#include "gpu/gl.hpp"

#include "buffer.hpp"
#include "program.hpp"
#include "gl_util.hpp"

StarBuffer::StarBuffer(GLProgram &prog, int count) :
		count(count) {
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	float data[] = {0.5, 0, 0, 100,
	                0, 0, 0, 10,
	                0, 0.5, 0, 2};

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	GLint pos_loc = glGetAttribLocation(prog.program_id(), "position");
	GLint size_loc = glGetAttribLocation(prog.program_id(), "size");

	glEnableVertexAttribArray(pos_loc);
	glEnableVertexAttribArray(size_loc);

	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat),
		(GLvoid*) 0);
	glVertexAttribPointer(size_loc, 1, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat),
		(GLvoid*) (3*sizeof(GLfloat)));

	check_gl_error("Binding star buffer", 5005);

	glBindVertexArray(0);
}

StarBuffer::~StarBuffer() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
}

void StarBuffer::draw() {
	glBindVertexArray(this->vao);
	glDrawArrays(GL_POINTS, 0, 3);
	check_gl_error("Drawing star buffer", 5006);

	glBindVertexArray(0);
}

