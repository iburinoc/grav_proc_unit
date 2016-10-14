#pragma once

#include "base.hpp"

#include "gpu/cl.hpp"

class GPUBackend : public Backend {
	compute::context ctx;
	compute::command_queue cmd_q;

	compute::kernel gr_step_kern;
	compute::kernel gr_end_kern;

	compute::opengl_buffer pos;
	compute::opengl_buffer size;

	compute::buffer vel;
	compute::buffer mass;
	compute::buffer active;

	compute::buffer x0;
	compute::buffer v0;
	compute::buffer x1;
	compute::buffer v1;

	compute::buffer va;
	compute::buffer aa;

	virtual void init();

	void init_particles();

	void set_base_kernel_args();

	void rk_step(float dt, int num);
	void rk_end(float dt);

public:
	GPUBackend(int count, std::unique_ptr<ParticleGen> p_gen);
	virtual ~GPUBackend();

	virtual void update(float dt);
};

