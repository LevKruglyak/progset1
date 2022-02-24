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

		float random_interval_iterative2() {
			float res;
			*((uint32_t *)&res) = (rand() >> 9) | 0x3f800000;
			return res - 1.0f;
		}

		Edge(uint32_t u, uint32_t v){
			float w = (float) rand()/RAND_MAX;//random_interval_iterative2();//((float) (rand() % 32767))/32767;
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

		int highestPowerof2(int n) //https://www.geeksforgeeks.org/highest-power-2-less-equal-given-number/
		{
    		int res = 0;
    		for (int i=n; i>=1; i--)
    		{
        		// If i is a power of 2
        		if ((i & (i-1)) == 0)
        		{
            		res = i;
            		break;
        		}
    		}
    		return res;
		}

		Graph(uint32_t V){
			this->V = V;
			//uint64_t denom = V*(V-1)+1;
			//float lim = 2*(2*V)/denom;
			//float lim = 1;
			float x = (float) highestPowerof2(V);
			float lim = 4.0*x/((float)V);// 4.0/((float)V); = expected value approx of largest edge
			if (V < 16){
				lim = 1;
			}
			//float edge_sum = 0;
			for (uint32_t i = 0; i < V; i++){
				//float edge_sum = 0;
				for (uint32_t j = i+1; j < V; j++){
					Edge e = *(new Edge(i, j));
					if (e.weight > lim){
						continue;
					}
					Q.push(e);
					//edge_sum += e.weight;
					//Q2.push(e);
				}
				//cout << "avg edge: " << edge_sum/(this->V-i-1) << ' ';
			}
			//cout << "avg edge: " << edge_sum/((this->V-1)*this->V/2.0) << ' ';
		}
	
	float kruskals();
	uint32_t find(uint32_t u);
	uint32_t union_(uint32_t i, uint32_t j);
};
