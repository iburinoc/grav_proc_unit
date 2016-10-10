#pragma once

#include <utility>

#include <boost/compute/core.hpp>
#include <boost/compute/exception/opencl_error.hpp>
#include <boost/compute/interop/opengl.hpp>

namespace compute = boost::compute;

std::pair<compute::context, compute::command_queue> init_cl();

