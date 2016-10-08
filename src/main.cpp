#include <memory>

#include <stdint.h>
#include <unistd.h>

#include "gpu/gl.hpp"

#include "gl_util.hpp"
#include "options.hpp"
#include "grav_proc_unit.hpp"
#include "backend.hpp"
#include "cl_util.hpp"

int main(int argc, char **argv) {
	process_args(argc, argv);

	GLFWwindow *window = init_gl();
	if(!window) {
		return 1;
	}

	auto backend = get_backend_from_opts();

	compute::context ctx = get_glcl_context();

	GravProcUnit gpu(window, std::move(backend));

	gpu.main_loop();

	end_gl(window);

	return 0;
}

