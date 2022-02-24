#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
using namespace std;

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


class Edge {
	public:
		uint32_t u;
		uint32_t v;
		float weight;

		Edge(uint32_t u, uint32_t v){
			float w = ((float) (rand() % 32767))/32767;
			this->u = u; this->v = v; this->weight = w;
		}
};

struct CustomCompare
{
    bool operator()(Edge e1, Edge e2)
    {
        return e1.weight > e2.weight;
    }
};

class Graph {
	public:
		uint32_t V;
		unordered_map<uint32_t, uint32_t> set_ids;
		unordered_map<uint32_t, unordered_set<uint32_t>> sets;
		priority_queue<Edge,vector<Edge>, CustomCompare> Q;
		priority_queue<Edge, vector<Edge>, CustomCompare> Q2;

		Graph(uint32_t V){
			this->V = V;
			uint32_t k = 100; //random test val
			float lim = 1.2*k/this->V;
			for (uint32_t i = 0; i < V; i++){
				for (uint32_t j = i+1; j < V; j++){
					Edge e = *(new Edge(i, j));
					//if (e.weight > lim){
					//	continue;
					//}
					Q.push(e);
					//Q2.push(e);
				}
			}
		}
	
	float kruskals();
	uint32_t find(uint32_t u);
	uint32_t union_(uint32_t i, uint32_t j);
};
