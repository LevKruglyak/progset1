#include "graphs.hpp"
#include "helpers.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

float AbstractGraph::average_minimum_weight() {
	std::unordered_set<uint32_t> outwards;
	for (int i = 1; i < this->V; ++i) {
		outwards.insert(i);
	}

	uint32_t current_node = 0;
	float average = 0.0;

	while (!outwards.empty()) {
		float minimum_weight = -1;
		uint32_t minimum_node = -1;

		for (auto &node : outwards) {
			float weight = this->weight(current_node, node);
			//  We can replace this with a call to rand() assuming we only visit each edge once
			if (minimum_node == -1 || weight < minimum_weight) {
				minimum_weight = weight;
				minimum_node = node;
			}
		}

		average += minimum_weight;
		current_node = minimum_node;

		outwards.erase(minimum_node);
	}

	return average / (this->V - 1);
}

float RandomCompleteGraph::weight(uint32_t u, uint32_t v) {
	assert(u < this->V && v < this->V);

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

	return random_interval(seed + hash_1(l + hash_2(h)));
}
