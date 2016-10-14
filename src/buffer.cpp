#include <vector>

#include "gpu/gl.hpp"
#include "gpu/glm.hpp"

#include "buffer.hpp"
#include "program.hpp"
#include "gl_util.hpp"

StarBuffer::StarBuffer(GLProgram &prog,
		const std::shared_ptr<Backend> &bk) :
		bk(bk) {
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->pvbo);
	glGenBuffers(1, &this->svbo);
	glGenBuffers(1, &this->ebo);

	check_gl_error("Creating buffers", 3200);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

	GLubyte inds[] = {0, 1, 2, 0, 2, 3};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds,
		GL_STATIC_DRAW);

	GLint pos_loc = glGetAttribLocation(prog.program_id(), "position");
	GLint size_loc = glGetAttribLocation(prog.program_id(), "size");

	glBindBuffer(GL_ARRAY_BUFFER, this->pvbo);
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, bk->pos_stride(),
		(GLvoid*) 0);

	glVertexAttribDivisor(pos_loc, 1);

	glBindBuffer(GL_ARRAY_BUFFER, this->svbo);
	glEnableVertexAttribArray(size_loc);
	glVertexAttribPointer(size_loc, 1, GL_FLOAT, GL_FALSE, 1*sizeof(GLfloat),
		(GLvoid*) 0);

	glVertexAttribDivisor(size_loc, 1);

	check_gl_error("Binding star buffer", 5005);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bk->set_vbos(this->pvbo, this->svbo);
}

StarBuffer::~StarBuffer() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->pvbo);
	glDeleteBuffers(1, &this->svbo);
	glDeleteBuffers(1, &this->ebo);
}

void StarBuffer::draw() {
	glBindVertexArray(this->vao);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*) 0,
		bk->get_count());
	check_gl_error("Drawing star buffer", 5006);

	glBindVertexArray(0);
}

GLuint StarBuffer::get_pvbo() const {
	return this->pvbo;
}

GLuint StarBuffer::get_svbo() const {
	return this->svbo;
}

