#include <tuple>

#include "gpu/gl.hpp"

#include "gpu.hpp"

#include "../cl_util.hpp"
#include "../kernels.hpp"

using namespace compute;

GPUBackend::GPUBackend(int count) : Backend(count) {
	std::tie(this->ctx, this->cmd_q) = init_cl();

	auto prog = create_program(GRAV_RK4_SRC, this->ctx);

	this->gr_step_kern = prog.create_kernel("gr_step");
	this->gr_end_kern = prog.create_kernel("gr_end");
}

GPUBackend::~GPUBackend() {}

void GPUBackend::init_buffers() {
	this->pos = opengl_buffer(ctx, sb->get_pvbo());
	this->size = opengl_buffer(ctx, sb->get_svbo());

	size_t base = sizeof(cl_float) * this->count;

	this->vel = buffer(ctx, base * 3);
	this->m = buffer(ctx, base);
	this->active = buffer(ctx, sizeof(cl_uchar) * this->count);

	this->x0 = buffer(ctx, base * 3);
	this->v0 = buffer(ctx, base * 3);
	this->x1 = buffer(ctx, base * 3);
	this->v1 = buffer(ctx, base * 3);

	this->va = buffer(ctx, base * 3);
	this->aa = buffer(ctx, base * 3);
}

void GPUBackend::set_buffer(StarBuffer *sb) {
	Backend::set_buffer(sb);

	glBindBuffer(GL_ARRAY_BUFFER, sb->get_pvbo());
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * 3 * this->count,
		NULL,
		GL_DYNAMIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, sb->get_svbo());
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * this->count,
		NULL,
		GL_DYNAMIC_DRAW);

	this->init_buffers();
}

void GPUBackend::update(float dt) {
	opengl_enqueue_acquire_buffer(this->pos, this->cmd_q);
	opengl_enqueue_acquire_buffer(this->size, this->cmd_q);

	vec3 empty(0.0f, 0.0f, 0.0f);

	this->cmd_q.enqueue_fill_buffer(this->va, value_ptr(empty))

	opengl_enqueue_release_buffer(this->pos, this->cmd_q);
	opengl_enqueue_release_buffer(this->size, this->cmd_q);
}

