#pragma once

#include "base.hpp"

#include "gpu/cl.hpp"

class GPUBackend : public Backend {
	compute::context ctx;
	compute::command_queue cmd_q;

	compute::kernel kern;

	compute::opengl_buffer pos;
	compute::opengl_buffer size;

	compute::buffer vel;
	compute::buffer m;
	compute::buffer active;

	compute::buffer x0;
	compute::buffer v0;
	compute::buffer x1;
	compute::buffer v1;

	compute::buffer va;
	compute::buffer aa;

	void init();

public:
	GPUBackend(int count);
	virtual ~GPUBackend();

	virtual void set_buffer(StarBuffer *sb);
	virtual void update(float dt);
};

