#include <string>
#include <iostream>
#include <vector>
#include <exception>

#include "opencl/opencl.h"

#include "cl_util.hpp"

static std::string get_platform_property(cl_platform_id pid,
		cl_platform_info param_name) {
	cl_int status;

	size_t size;
	status = clGetPlatformInfo(pid, param_name,
		0, nullptr, &size);

	std::string s;
	s.resize(size);

	status = clGetPlatformInfo(pid, param_name, size, &s[0], nullptr);

	return s;
}

static bool gl_sharing_capable(cl_platform_id pid) {
	std::string exts = get_platform_property(pid, CL_PLATFORM_EXTENSIONS);

#ifdef __APPLE__
	const std::string target = "cl_APPLE_gl_sharing";
#else
	const std::string target = "cl_khr_gl_sharing";
#endif

	return exts.find(target) != std::string::npos;
}

cl_platform_id get_platform() {
	cl_int status;

	cl_uint num_platforms;
	status = clGetPlatformIDs(0, nullptr, &num_platforms);

	std::vector<cl_platform_id> platforms(num_platforms);
	status = clGetPlatformIDs(num_platforms, &platforms[0], nullptr);

	cl_platform_id pid;

	cl_uint i;
	for(i = 0; i < num_platforms; i++) {
		// check for extension
		if(gl_sharing_capable(platforms[i])) {
			pid = platforms[i];
			break;
		}
	}

	if(i == num_platforms) {
		throw new std::runtime_error("No suitable OpenCL platforms");
	}

	std::cerr << get_platform_property(pid, CL_PLATFORM_VERSION)
		<< std::endl;

	return pid;
}

void init_opencl() {
	cl_platform_id pid = get_platform();

}

