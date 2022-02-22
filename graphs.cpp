#include "graphs.hpp"
#include "helpers.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_set>

float AbstractGraph::average_minimum_weight() {
	std::unordered_set<uint32_t> outwards;
	for (int i = 1; i < this->V; ++i) {
		outwards.insert(i);
	}

	uint32_t current_node = 0;
	float minimum_spanning_tree[this->V];

	while (!outwards.empty()) {
		float minimum_weight = -1;
		uint32_t minimum_node = -1;
		for (const auto &m : outwards) {
			float w = this->weight(current_node, m);
			if (minimum_node == -1 || w < minimum_weight) {
				minimum_weight = w;
				minimum_node = m;
			}
		}
		minimum_spanning_tree[minimum_node] = minimum_weight;
		outwards.erase(minimum_node);
		current_node = minimum_node;
	}

	float avg = 0;
	for (int i = 1; i < this->V; ++i) {
		avg += minimum_spanning_tree[i];
	}

	return avg / (this->V - 1);
}

/*
float prim(RandomCompleteGraph *G) {
	unordered_set<unsigned int> outwards; // is this the best ds to use?
	for (int i = 1; i <= G->V; i++) {	  // cleaner?
		outwards.insert(i);
	}
	unsigned int n = 0;
	float mst[G->V];
	while (!outwards.empty()) {
		float min_w = -1;
		unsigned int min_node = -1;
		for (const auto &m : outwards) {
			float w = G->weight(n, m);
			if (min_node == -1 || w < min_w) {
				min_w = w;
				min_node = m;
			}
		}
		mst[min_node] = min_w;
		outwards.erase(min_node);
		n = min_node;
	}
	return max_element(mst, mst + G->V);
}
*/

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

	return random_interval(seed + hash_1(l + hash_1(h)));
}
