#include <iostream>
#include <tuple>

#include "gpu/glm.hpp"

#include "../cl_util.hpp"
#include "../kernels.hpp"

#include "gpu.hpp"

using namespace compute;

GPUBackend::GPUBackend(int count) : Backend(1, sizeof(cl_float3)) {
	std::tie(this->ctx, this->cmd_q) = init_cl();

	auto prog = create_program(GRAV_RK4_SRC, this->ctx);

	this->gr_step_kern = prog.create_kernel("gr_step");
	this->gr_end_kern = prog.create_kernel("gr_end");
}

GPUBackend::~GPUBackend() {}

static void init_buffer_size(GLuint buf, size_t size) {
	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER,
		size,
		NULL,
		GL_DYNAMIC_DRAW);
}

void GPUBackend::init() {
	size_t base = sizeof(cl_float) * this->count;

	init_buffer_size(this->pvbo, base * 4);
	this->pos = opengl_buffer(ctx, this->pvbo);
	init_buffer_size(this->svbo, base);
	this->size = opengl_buffer(ctx, this->svbo);


	this->vel = buffer(ctx, base * 4);
	this->m = buffer(ctx, base);
	this->active = buffer(ctx, sizeof(cl_uchar) * this->count);

	this->x0 = buffer(ctx, base * 4);
	this->v0 = buffer(ctx, base * 4);
	this->x1 = buffer(ctx, base * 4);
	this->v1 = buffer(ctx, base * 4);

	this->va = buffer(ctx, base * 4);
	this->aa = buffer(ctx, base * 4);

	this->init_particles();

	this->set_base_kernel_args();
}

void GPUBackend::init_particles() {
	cl_float3 pos = (cl_float3) {1.0f, 1.0f, 1.0f};
	cl_float3 vel = (cl_float3) {0.0f, 0.1f, 0.0f};
	cl_float size = 1.0f;

	opengl_enqueue_acquire_buffer(this->pos, this->cmd_q);
	opengl_enqueue_acquire_buffer(this->size, this->cmd_q);

	this->cmd_q.enqueue_write_buffer(this->pos, 0, sizeof(pos), pos.s);
	this->cmd_q.enqueue_write_buffer(this->vel, 0, sizeof(vel), vel.s);
	this->cmd_q.enqueue_write_buffer(this->size, 0, sizeof(size), &size);

	cl_uchar true_val = 1;
	this->cmd_q.enqueue_fill_buffer(this->active, &true_val,
		sizeof(cl_uchar), 0, sizeof(cl_uchar) * this->count);

	opengl_enqueue_release_buffer(this->pos, this->cmd_q);
	opengl_enqueue_release_buffer(this->size, this->cmd_q);
	this->cmd_q.flush();
}

void GPUBackend::set_base_kernel_args() {
	kernel &kern = this->gr_step_kern;
	int i = 0;

	kern.set_arg(i++, this->count);
	i++; // dt
	i++; // ratio

	kern.set_arg(i++, this->pos);
	kern.set_arg(i++, this->vel);
	i++; // p1
	i++; // v1

	kern.set_arg(i++, this->size);
	kern.set_arg(i++, this->active);

	i++; // p2
	i++; // v2
	kern.set_arg(i++, this->va);
	kern.set_arg(i++, this->aa);

	kernel &ekern = this->gr_end_kern;
	i = 0;

	ekern.set_arg(i++, this->count);
	i++; // dt

	ekern.set_arg(i++, this->active);

	ekern.set_arg(i++, this->va);
	ekern.set_arg(i++, this->aa);

	ekern.set_arg(i++, this->pos);
	ekern.set_arg(i++, this->vel);
}

static void enqueue_kernel(command_queue &cmd_q, kernel &kern, size_t count) {
	const size_t WORKGROUP = 32;
	size_t total = ((count + WORKGROUP-1) / WORKGROUP) * WORKGROUP;
	cmd_q.enqueue_1d_range_kernel(kern, 0, total, WORKGROUP);
}

void GPUBackend::rk_step(float dt, int num) {
	kernel &kern = this->gr_step_kern;

	kern.set_arg(1, dt);

	switch(num) {
		case 1:
		case 4: kern.set_arg(2, 1.0f); break; // ratio
		case 2:
		case 3: kern.set_arg(2, 2.0f); break; // ratio
	}

	switch(num) {
	case 1:
		kern.set_arg(5, this->pos);
		kern.set_arg(6, this->vel);
		kern.set_arg(9, this->x0);
		kern.set_arg(10, this->v0);
		break;
	case 2:
	case 4:
		kern.set_arg(5, this->x0);
		kern.set_arg(6, this->v0);
		kern.set_arg(9, this->x1);
		kern.set_arg(10, this->v1);
		break;
	case 3:
		kern.set_arg(5, this->x1);
		kern.set_arg(6, this->v1);
		kern.set_arg(9, this->x0);
		kern.set_arg(10, this->v0);
		break;
	}

	enqueue_kernel(this->cmd_q, kern, this->count);
}

void GPUBackend::rk_end(float dt) {
	kernel &ekern = this->gr_end_kern;

	ekern.set_arg(1, dt);

	enqueue_kernel(this->cmd_q, ekern, this->count);
}

void GPUBackend::update(float dt) {
	opengl_enqueue_acquire_buffer(this->pos, this->cmd_q);
	opengl_enqueue_acquire_buffer(this->size, this->cmd_q);

	cl_float3 empty = (cl_float3) {0.0f, 0.0f, 0.0};

	this->cmd_q.enqueue_fill_buffer(this->va, empty.s,
		sizeof(empty), 0, sizeof(empty) * this->count);
	this->cmd_q.enqueue_fill_buffer(this->aa, empty.s,
		sizeof(empty), 0, sizeof(empty) * this->count);

	this->rk_step(dt, 1);
	this->rk_step(dt, 2);
	this->rk_step(dt, 3);
	this->rk_step(dt, 4);
	this->rk_end(dt);

	cl_float3 pos;
	cl_float3 vel;

	this->cmd_q.enqueue_read_buffer(this->pos, 0,sizeof(pos), pos.s);
	this->cmd_q.enqueue_read_buffer(this->vel, 0,sizeof(vel), vel.s);

	std::cout << pos.x << "," << pos.y << "," << pos.z << std::endl;
	std::cout << vel.x << "," << vel.y << "," << vel.z << std::endl;

	opengl_enqueue_release_buffer(this->pos, this->cmd_q);
	opengl_enqueue_release_buffer(this->size, this->cmd_q);
}

