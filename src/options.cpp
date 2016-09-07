#include <iostream>

#include <tclap/CmdLine.h>

#include "gpu/glm.hpp"

#include "options.hpp"

Options options;

void process_args(int argc, char **argv) {
	try {
		TCLAP::CmdLine cmd("Render an n-body simulation",
		                   ' ',
		                   "0.1.0");
		TCLAP::ValueArg<int> widthArg("W", "width", "Window width",
		                              false, 800, "int", cmd);
		TCLAP::ValueArg<int> heightArg("H", "height", "Window height",
		                              false, 800, "int", cmd);

		cmd.parse(argc, argv);

		options.dimensions = ivec2(widthArg.getValue(),
		                           heightArg.getValue());
	} catch(TCLAP::ArgException &e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId()
		          << std::endl;
	}
}

