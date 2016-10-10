#include <iostream>
#include <utility>

#include "gpu/cl.hpp"

#include "cl_util.hpp"

using namespace compute;

// Call after OpenGL context has been created
context get_glcl_context() {
	return opengl_create_shared_context();
}

std::pair<context, command_queue> init_cl() {
	context ctx = get_glcl_context();

	std::cerr << "OpenCL Version: " << 
		ctx.get_device().get_info<CL_DEVICE_VERSION>() << std::endl;

	command_queue q(ctx, ctx.get_device());

	return std::make_pair(std::move(ctx), std::move(q));
}

program create_program(const std::string &src, const context &ctx) {
	program prg = program::create_with_source(src, ctx);
	try {
		prg.build();
	} catch(const opencl_error &e) {
		std::cerr << prg.build_log() << std::endl;
		throw;
	}

	return prg;
}

