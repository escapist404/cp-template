#ifndef FUNCTIONAL_GRAPH_HPP
#define FUNCTIONAL_GRAPH_HPP

#include <cassert>
#include <cstdint>
#include <queue>
#include <vector>

class FunctionalGraph {
	enum { LOG = 64 };
	int n;
	std::vector<std::vector<int>> up;
	std::vector<int> cycle, distance, length;

public:
	FunctionalGraph(const std::vector<int>& next) : n(next.size()), up(LOG, next),
		cycle(n, -1), distance(n), length(n) {
		std::vector<int> indegree(n);
		std::vector<std::vector<int>> reverse(n);
		for (int i = 0; i < n; ++i) {
			assert(0 <= next[i] && next[i] < n);
			++indegree[next[i]];
			reverse[next[i]].push_back(i);
		}
		std::queue<int> queue;
		for (int i = 0; i < n; ++i) if (!indegree[i]) queue.push(i);
		while (!queue.empty()) {
			int u = queue.front(); queue.pop();
			int v = next[u];
			if (!--indegree[v]) queue.push(v);
		}
		int id = 0;
		for (int i = 0; i < n; ++i) if (indegree[i] && cycle[i] == -1) {
			std::vector<int> nodes;
			for (int v = i;; v = next[v]) {
				cycle[v] = id;
				nodes.push_back(v);
				if (next[v] == i) break;
			}
			for (int v : nodes) length[v] = nodes.size();
			++id;
		}
		std::queue<int> order;
		for (int i = 0; i < n; ++i) if (cycle[i] != -1) order.push(i);
		while (!order.empty()) {
			int u = order.front(); order.pop();
			for (int v : reverse[u]) if (cycle[v] == -1) {
				cycle[v] = cycle[u];
				distance[v] = distance[u] + 1;
				length[v] = length[u];
				order.push(v);
			}
		}
		for (int k = 1; k < LOG; ++k)
			for (int i = 0; i < n; ++i)
				up[k][i] = up[k - 1][up[k - 1][i]];
	}

	int jump(int vertex, std::uint64_t steps) {
		assert(0 <= vertex && vertex < n);
		for (int k = 0; steps; ++k, steps >>= 1)
			if (steps & 1) vertex = up[k][vertex];
		return vertex;
	}

	int component(int vertex) { return cycle.at(vertex); }
	int distance_to_cycle(int vertex) { return distance.at(vertex); }
	int cycle_length(int vertex) { return length.at(vertex); }
};

#endif
