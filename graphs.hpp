#pragma once

#include <cstdint>

/// Represents a weighted undirected graph
class AbstractGraph {
  public:
	uint32_t V;

	/// Returns edge between u and v
	virtual float weight(uint32_t u, uint32_t v) { return 0; };

	float average_minimum_weight();
};

class RandomCompleteGraph : public AbstractGraph {
  public:
	RandomCompleteGraph(uint32_t V, uint32_t seed) {
		this->V = V;
		this->seed = seed;
	}

	float weight(uint32_t u, uint32_t v);

	uint32_t seed;
};
