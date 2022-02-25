#include "graphs.hpp"
#include "helpers.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

Graph::Graph(uint32_t V) {
	auto gen = RandomFloatGenerator();
	this->V = V;

	float lim = 16.0 * power_of_two(V) / ((float)V);

	if (V < 16) {
		lim = 1;
	}

	for (uint32_t i = 0; i < V; i++) {
		for (uint32_t j = i + 1; j < V; j++) {
			Edge e(i, j, gen.random_float());
			if (e.weight < lim) {
				Q.push(e);
			}
		}
	}
}

// populated priority queue passed in
float Graph::kruskals() {
	float sum = 0;
	uint32_t m = 0;
	for (uint32_t i = 0; i < this->V; i++) {
		this->set_ids[i] = i;
		this->sets[i] = {i};
	}
	while (this->sets[m].size() < this->V) {
		Edge e = this->Q.top();
		this->Q.pop();
		if (find(e.u) == find(e.v)) {
			continue;
		}
		m = union_(find(e.u), find(e.v));
		sum += e.weight;
		// std::cout << e.u << ' ' << e.v << ' ' << e.weight << ' ';
	}
	return sum;
}

uint32_t Graph::find(uint32_t u) { return this->set_ids[u]; }

uint32_t Graph::union_(uint32_t i, uint32_t j) {
	if (this->sets[i].size() < this->sets[j].size()) {
		uint32_t k = i;
		i = j;
		j = k;
	}
	this->sets[i].insert(this->sets[j].begin(), this->sets[j].end());
	for (uint32_t node : this->sets[j]) {
		this->set_ids[node] = i;
	}
	return i;
}

struct Node {
	static uint32_t counter;

	unique_ptr<vector<Node *>> children;
	vector<float> *center;
	float radius;
	float diagonal_radius;
	uint32_t representative = -1;
	unsigned int level;
	uint32_t uuid;

	void split(unsigned int d) {
		unsigned int children_count = 1 << d;
		children = make_unique<vector<Node *>>(children_count);

		// Calculate center
		for (unsigned int index = 0; index < (1 << d); ++index) {
			Node *child = new Node();

			child->radius = this->radius / 2.0f;
			child->diagonal_radius = this->diagonal_radius / 2.0f;
			child->center = new vector<float>(d);
			child->level = this->level + 1;

			Node::counter+= 1;
			child->uuid = counter;

			for (unsigned int coord = 0; coord < d; ++coord) {
				if ((index & (1 << coord)) == 0) {
					// Left side
					child->center->at(coord) = this->center->at(coord) - child->radius;
				} else {
					// Right side
					child->center->at(coord) = this->center->at(coord) + child->radius;
				}
			}

			children->at(index) = child;
		}
	}

	unsigned int get_index(deque<float>::iterator point, unsigned int d) {
		unsigned int index = 0;

		for (unsigned int coord = 0; coord < d; ++coord) {
			if (*point > this->center->at(coord)) {
				index |= (1 << coord);
			}
			++point;
		}

		return index;
	}

	void insert(uint32_t point, deque<float> *points, unsigned int d) {
		deque<float>::iterator point_iterator = points->begin() + d * point;

		if (leaf()) {
			if (empty()) {
				this->representative = point;
			} else {
				split(d);
				insert(this->representative, points, d);
				insert(point, points, d);
			}
		} else {
			children->at(get_index(point_iterator, d))->insert(point, points, d);
		}
	}

	bool leaf() { return children == nullptr; }
	bool empty() { return representative == -1; }

	void print_list() {
		if (leaf() && !empty()) {
			cout << representative << " ";
		}

		if (!leaf()) {
			for (auto &val : *children) {
				val->print_list();
			}
		}
	}

	void print() {
		if (leaf() && !empty()) {
			cout << "level " << level << endl;
			cout << "   value " << representative << endl;
		}
		if (!leaf()) {
			for (auto &val : *children) {
				val->print();
			}
		}
	}
};

//https://stackoverflow.com/questions/15160889/how-can-i-make-an-unordered-set-of-pairs-of-integers-in-c
struct pair_hash {
    inline size_t operator()(const pair<Node*, Node*> & p) const {
		Node* n1 = p.first;
		Node* n2 = p.second;
		if (n1->uuid > n2->uuid){
			n1 = p.second;
			n2 = p.first;
		}
		return n1->uuid*31+n2->uuid;
	}
};

struct pair_equality{
	inline bool operator() (const pair<Node*, Node*> & p1, const pair<Node*, Node*> & p2) const{
    	return false; //FIX TO USE HASH
	}
};

float dist_sq(vector<float> *u, vector<float> *v, unsigned int d) {
	float sum = 0.0;
	for (unsigned int coord = 0; coord < d; ++coord) {
		sum += (u->at(coord) - v->at(coord)) * (u->at(coord) - v->at(coord));
	}
	return sum;
}

bool well_separated(Node *u, Node *v, uint32_t s, deque<float> *points, unsigned int d) {
	if (u->leaf() && v->leaf()) {
		return true;
	}

	vector<float> *center_u = u->center;
	vector<float> *center_v = v->center;

	float max_radius = max(u->diagonal_radius, v->diagonal_radius);

	if (u->leaf()) {
		center_u = new vector<float>(d);
		for (unsigned int coord = 0; coord < d; ++coord) {
			center_u->at(coord) = points->at(u->representative * d + coord);
		}
		max_radius = v->diagonal_radius;
	}

	if (v->leaf()) {
		center_v = new vector<float>(d);
		for (unsigned int coord = 0; coord < d; ++coord) {
			center_v->at(coord) = points->at(v->representative * d + coord);
		}
		max_radius = u->diagonal_radius;
	}

	return dist_sq(center_u, center_v, d) >= (s + 2) * (s + 2) * max_radius * max_radius;
}

//currently checked lets us add elements multiple times because pair_equality is incorrect
void ws_pairs(Node *u, Node *v, uint32_t s, deque<pair<Node *, Node *>> *wspd, deque<float> *points,
			  unsigned int d, unordered_set< pair<Node*, Node*>,  pair_hash, pair_equality> *checked) {

	cout << "call (" << u->uuid << "," << v->uuid << ")  " << u->level << "[";
	u->print_list();
	cout << "]  " << v->level << "[";
	v->print_list();
	cout << "] " << endl;

	pair<Node*, Node*> p(u, v);
	checked->insert(p);
	if (well_separated(u, v, s, points, d)) {
		// cout << "   well separated " << endl;
		wspd->push_back(p);
	} else {
		if (u->level > v->level || u->leaf()) {
			// Swap u and v
			Node *w = u;
			u = v;
			v = w;
		}
		if (!u->leaf()) {
			for (auto &child : *u->children) {
				if (v->empty() || child->empty() ||
					(v->leaf() && child->leaf() && child->representative == v->representative)) {
					continue;
				}

				// Avoid duplicate pairs
				// cout << child->count << " " << v->count << endl;
				//if (child->level != v->level || child->representative <= v->representative) {
				pair<Node*, Node*> p(child, v);
				if (checked->find(p) == checked->end()){
					ws_pairs(child, v, s, wspd, points, d, checked);
				}
			}
		}
	}
}

uint32_t Node::counter=0;

float emst(unsigned int n, unsigned int d) {
	assert(d > 1 && d <= 4);

	// Generate random list of points
	RandomFloatGenerator random;
	deque<float> *points = new deque<float>(n * d);
	for (int point_coord = 0; point_coord < n * d; ++point_coord) {
		points->at(point_coord) = random.random_float();
	}

	// Populate quad tree
	Node *root = new Node();
	root->center = new vector<float>(d);
	fill(root->center->begin(), root->center->end(), 0.5f);
	root->radius = 0.5f;
	root->diagonal_radius = sqrt(0.5f);

	for (uint32_t point = 0; point < n; ++point) {
		/*
		cout << point << " point ";
		for (unsigned int i = 0; i < d; ++i) {
			cout << points->at(point * d + i) << " ";
		}
		cout << endl;
		*/

		root->insert(point, points, d);
	}

	cout << "counter: " << Node::counter << endl;

	cout << "finished building quad tree" << endl;

	// Build a 2-WSPD
	auto wspd = new deque<pair<Node *, Node *>>();
	auto checked = new unordered_set< pair<Node*, Node*>,  pair_hash, pair_equality>;
	ws_pairs(root, root, 0, wspd, points, d, checked);

	cout << wspd->size() << endl;
	for (auto &val : *wspd) {
		cout << "pair ";
		val.first->print_list();
		cout << "   |   ";
		val.second->print_list();
		cout << endl;
	}

	// Cleanup
	delete root;
	delete points;

	return 1.0f;
}
