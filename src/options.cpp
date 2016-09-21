#include <iostream>

#include <stdint.h>

#include <tclap/CmdLine.h>

#include "gpu/glm.hpp"

#include "options.hpp"

Options options;

void process_args(int argc, char **argv) {
	try {
		TCLAP::CmdLine cmd("GPU: Render an n-body simulation",
		                   ' ',
		                   "0.1.0");
		TCLAP::ValueArg<int> widthArg("W", "width", "Window width",
		                              false, 800, "int", cmd);
		TCLAP::ValueArg<int> heightArg("H", "height", "Window height",
		                              false, 800, "int", cmd);
		TCLAP::ValueArg<int> pcountArg("c", "count", "Particle count",
		                               false, 10000, "int", cmd);
		TCLAP::ValueArg<unsigned long> seedArg("s", "seed",
		                                       "Random seed", false, 0,
						       "long int", cmd);
		TCLAP::SwitchArg debugArg("d", "debug", "Activates debug mode",
		                          cmd);

		cmd.parse(argc, argv);

		options.dimensions = ivec2(widthArg.getValue(),
		                           heightArg.getValue());

		options.particle_count = pcountArg.getValue();
		options.seed = seedArg.getValue();

		options.debug = debugArg.getValue();
	} catch(TCLAP::ArgException &e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId()
		          << std::endl;
	}
}

