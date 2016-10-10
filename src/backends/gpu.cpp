#include <tuple>

#include "gpu.hpp"

#include "../cl_util.hpp"
#include "../kernels.hpp"

using namespace compute;

GPUBackend::GPUBackend(int count) : Backend(count) {
	std::tie(this->ctx, this->cmd_q) = init_cl();

	this->kern = create_kernel(GRAV_RK4_SRC, this->ctx);
}

GPUBackend::~GPUBackend() {}

void GPUBackend::init() {
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

	this->init();
}

void GPUBackend::update(float dt) {
	/* do shit */
}

