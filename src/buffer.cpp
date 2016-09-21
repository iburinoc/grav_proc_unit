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

	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ebo);

	check_gl_error("Creating buffers", 3200);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

	float data[] = {0.5, 0, 0, 0.5,
	                0, 0, 0, 0.4,
	                0, 0.5, 0, 0.5};

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	GLubyte inds[] = {0, 1, 2, 0, 2, 3};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds,
		GL_STATIC_DRAW);

	GLint pos_loc = glGetAttribLocation(prog.program_id(), "position");
	GLint size_loc = glGetAttribLocation(prog.program_id(), "size");

	glEnableVertexAttribArray(pos_loc);
	glEnableVertexAttribArray(size_loc);

	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat),
		(GLvoid*) 0);
	glVertexAttribPointer(size_loc, 1, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat),
		(GLvoid*) (3*sizeof(GLfloat)));

	glVertexAttribDivisor(pos_loc, 1);
	glVertexAttribDivisor(size_loc, 1);

	check_gl_error("Binding star buffer", 5005);

	glBindVertexArray(0);
}

StarBuffer::~StarBuffer() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo);
}

// Take position and size data as vec4's to ensure the array is packed
void StarBuffer::set_vertices(std::vector<vec4> &data) {
	glBindVertexArray(this->vao);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * data.size(), &data[0],
		GL_DYNAMIC_DRAW);
	this->count = data.size();

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

