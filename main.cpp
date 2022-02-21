#include "graphs.hpp"
#include "helpers.hpp"
#include <iostream>
#include <string.h>

static void usage() {
	fprintf(stderr, "\
    Usage: ./randmst [DEBUG] [NUMPOINTS] [NUMTRIALS] [DIMENSION]\n");
	exit(1);
}

int main(int argc, char *argv[]) {
	// Validate arguments
	if (argc == 5) {
		bool debug;
		unsigned int points, trials, dimension;

		// Ensure all parameters are integers
		if (is_integer_string(argv[1]) && is_integer_string(argv[2]) &&
			is_integer_string(argv[3]) && is_integer_string(argv[4])) {

			debug = (strtol(argv[1], nullptr, 10) != 0);

			points = (unsigned int)strtol(argv[2], nullptr, 10);
			trials = (unsigned int)strtol(argv[3], nullptr, 10);
			dimension = (unsigned int)strtol(argv[4], nullptr, 10);

			auto graph = RandomCompleteGraph(100, 0);
			std::cout << graph.weight(0, 1) << std::endl;
			std::cout << graph.weight(1, 0) << std::endl;
			std::cout << graph.weight(1, 2) << std::endl;

			exit(0);
		}
	}

	// Print usage
	usage();
}

