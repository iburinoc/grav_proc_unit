#include <vector>

#include "gpu/gl.hpp"
#include "gpu/glm.hpp"

#include "buffer.hpp"
#include "program.hpp"
#include "gl_util.hpp"

StarBuffer::StarBuffer(GLProgram &prog, int count) :
		count(count) {
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
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat),
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
}

StarBuffer::~StarBuffer() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->pvbo);
	glDeleteBuffers(1, &this->svbo);
	glDeleteBuffers(1, &this->ebo);
}

// Take position and size data as vec4's to ensure the array is packed
void StarBuffer::set_vertices(std::vector<vec3> &pos,
		std::vector<GLfloat> &size) {
	glBindVertexArray(this->vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->pvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * pos.size(), &pos[0],
		GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->svbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size.size(), &size[0],
		GL_DYNAMIC_DRAW);

	this->count = pos.size();

	check_gl_error("Loading data", 5007);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void StarBuffer::draw() {
	glBindVertexArray(this->vao);
	//glDrawArrays(GL_POINTS, 0, 3);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*) 0,
		this->count);
	check_gl_error("Drawing star buffer", 5006);

	glBindVertexArray(0);
}

