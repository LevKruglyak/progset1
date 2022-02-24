#include "graphs.hpp"
#include "helpers.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>

//experimentally determine that aprey's constant = the MST weight
//we can delete any edge weights which exceed aprey's constant*k/N? Where k is some as yet unknown factor...


void printqueue(priority_queue<Edge, vector<Edge>, CustomCompare> q) { // NB: pass by value so the print uses a copy
    while(!q.empty()) {
		std::cout << q.top().weight << ' ';
        //std::cout << q.top() << ' ';
        q.pop();
    }
    std::cout << '\n';
}

//populated priority queue passed in
float Graph::kruskals(){
    //printqueue(this->Q2);
    float sum = 0;
    uint32_t m = 0;
    for (uint32_t i=0; i < this->V; i++){
        this->set_ids[i] = i;
        this->sets[i] = {i};
    }
    while (this->sets[m].size() < this->V){
        Edge e = this->Q.top();
        this->Q.pop();
        if (find(e.u) == find(e.v)){
            continue;
        }
        m = union_(find(e.u), find(e.v));
        sum += e.weight;
    }
    return sum;
}


uint32_t Graph::find(uint32_t u){
    return this->set_ids[u];
}

uint32_t Graph::union_(uint32_t i, uint32_t j){
    if (this->sets[i].size() < this->sets[j].size()){
        uint32_t k = i; i = j; j = k;
    }
    this->sets[i].insert(this->sets[j].begin(), this->sets[j].end());
    for (uint32_t node : this->sets[j]){
        this->set_ids[node] = i;
    }
    return i;
}












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
