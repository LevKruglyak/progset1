#include "graphs.hpp"
#include "helpers.hpp"

float RandomCompleteGraph::weight(uint32_t u, uint32_t v) {
	if (u == v) {
		return 0;
	}

	uint32_t l, h;
	if (u > v) {
		l = v;
		h = u;
	} else {
		l = u;
		h = v;
	}

	return random_interval(seed + hash_1(l + hash_1(h)));
}
