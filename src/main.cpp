#include <stdint.h>
#include <unistd.h>

#include "gpu/gl.hpp"

#include "gl_util.hpp"
#include "options.hpp"

int main(int argc, char **argv) {
	process_args(argc, argv);

	GLFWwindow *window = init_gl();
	if(!window) {
		return 1;
	}


	end_gl(window);

	return 0;
}

