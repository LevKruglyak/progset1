#include "graphs.hpp"
#include "helpers.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <random>
#include <string.h>
#include <thread>
#include <vector>

static void usage() {
	fprintf(stderr, "\
    Usage: ./randmst [DEBUG] [NUMPOINTS] [NUMTRIALS] [DIMENSION]\n");
	exit(1);
}

void compute_minimum_weight(unsigned int points, unsigned int dimension, unsigned int seed,
							float *output) {
	switch (dimension) {
	case 0: // Randomly assigned weights
		*output = RandomCompleteGraph(points, seed).average_minimum_weight();
		break;
	default:
		break;
	}
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

			// Don't randonly seed the rng if in debug mode
			if (!debug) {
				srand(time(NULL));
			}

			auto results = new std::vector<float>(trials);

			// Don't use threads in debug mode
			if (!debug) {
				std::vector<std::thread> threads;

				for (unsigned int trial = 0; trial < trials; ++trial) {
					threads.push_back(std::thread(compute_minimum_weight, points, dimension,
												  (unsigned int)rand(), &results->data()[trial]));
				}

				for (auto &thr : threads) {
					thr.join();
				}
			} else {
				for (unsigned int trial = 0; trial < trials; ++trial) {
					compute_minimum_weight(points, dimension, (unsigned int)rand(),
										   &results->data()[trial]);
				}
			}

			// Calculate and display average
			float average = 0.0f;
			for (auto &result : *results) {
				average += result / trials;
			}

			std::cout << average << std::endl;

			exit(0);
		}
	}

	// Print usage
	usage();
}

