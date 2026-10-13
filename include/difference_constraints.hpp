#ifndef DIFFERENCE_CONSTRAINTS_HPP
#define DIFFERENCE_CONSTRAINTS_HPP

#include <cassert>
#include <queue>
#include <utility>
#include <vector>

// u -> v with weight w means x[v] <= x[u] + w.
template <typename T = long long>
struct DifferenceConstraint {
	int u, v;
	T weight;
};

template <typename T = long long>
std::pair<bool, std::vector<T>> difference_constraints(
		int n, const std::vector<DifferenceConstraint<T>>& edges) {
	assert(n >= 0);
	std::vector<std::vector<DifferenceConstraint<T>>> graph(n);
	for (const auto& edge : edges) {
		assert(0 <= edge.u && edge.u < n);
		assert(0 <= edge.v && edge.v < n);
		graph[edge.u].push_back(edge);
	}
	std::queue<int> queue;
	std::vector<bool> in_queue(n, true);
	std::vector<int> count(n);
	std::vector<T> value(n);
	for (int i = 0; i < n; ++i) queue.push(i);
	while (!queue.empty()) {
		const int u = queue.front();
		queue.pop();
		in_queue[u] = false;
		for (const auto& edge : graph[u]) if (value[edge.v] > value[u] + edge.weight) {
			value[edge.v] = value[u] + edge.weight;
			count[edge.v] = count[u] + 1;
			if (count[edge.v] >= n) return {false, {}};
			if (!in_queue[edge.v]) {
				in_queue[edge.v] = true;
				queue.push(edge.v);
			}
		}
	}
	return {true, std::move(value)};
}

#endif
