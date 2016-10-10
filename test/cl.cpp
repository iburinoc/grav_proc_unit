#include <iostream>

#include "gl_util.hpp"
#include "cl_util.hpp"
#include "options.hpp"

const std::string kern_src =
#include "vecAdd.cl"
;


float d1[65536];
float d2[65536];
float d3[65536];

bool equal(float a, float b) {
	return fabs(a-b) <= 1e-6;
}

int main(int argc, char **argv) {
	process_args(argc, argv);

	GLFWwindow *window = init_gl();
	auto tmp = init_cl();

	auto ctx = tmp.first;
	auto cmd_q = tmp.second;

	auto prog = create_program(kern_src, ctx);
	auto kern = prog.create_kernel("kern");

	for(int i = 0; i < 65536; i++) {
		d1[i] = (float)i;
		d2[i] = (float)i / 2.0;
	}

	compute::buffer b1(ctx, sizeof(d1));
	compute::buffer b2(ctx, sizeof(d2));
	compute::buffer b3(ctx, sizeof(d3));

	cmd_q.enqueue_write_buffer(b1, 0, sizeof(d1), d1);
	cmd_q.enqueue_write_buffer(b2, 0, sizeof(d2), d2);

	kern.set_arg(0, b1);
	kern.set_arg(1, b2);
	kern.set_arg(2, b3);
	kern.set_arg(3, 65536);

	cmd_q.enqueue_1d_range_kernel(kern, 0, 65536, 32);

	kern.set_arg(0, b3);
	kern.set_arg(1, b2);
	kern.set_arg(2, b1);
	kern.set_arg(3, 65536);

	cmd_q.enqueue_1d_range_kernel(kern, 0, 65536, 32);

	cmd_q.enqueue_read_buffer(b1, 0, sizeof(d3), d3);

	cmd_q.flush();

	for(int i = 0; i < 65536; i++) {
		if(!equal(d1[i] + d2[i] + d2[i], d3[i])) {
			std::cerr << i << " " << d3[i] << std::endl;
		}
	}

	end_gl(window);
}

