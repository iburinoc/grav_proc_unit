#pragma once

#include "gpu/cl.hpp"

compute::context get_glcl_context();

std::pair<compute::context, compute::command_queue> init_cl();

compute::kernel create_kernel(const std::string &src,
	const compute::context &ctx);

