#ifndef BIPARTITE_MATCHING_HPP
#define BIPARTITE_MATCHING_HPP

#include <algorithm>
#include <cassert>
#include <queue>
#include <utility>
#include <vector>

// Vertices on the left are [0, adj.size()), vertices on the right are
// [0, right_size). Hopcroft-Karp: O((V + E) sqrt(V)).
struct BipartiteMatchingResult {
	int size = 0;
	std::vector<int> left, right;
};

BipartiteMatchingResult bipartite_matching(
		const std::vector<std::vector<int>>& adj, int right_size) {
	assert(right_size >= 0);
	const int left_size = adj.size();
	std::vector<int> left_match(left_size, -1), right_match(right_size, -1);
	std::vector<int> distance(left_size), current(left_size);
	int shortest;
	auto dfs = [&](auto&& self, int u) -> bool {
		for (int& i = current[u]; i < (int)adj[u].size(); ++i) {
			const int v = adj[u][i];
			assert(0 <= v && v < right_size);
			if ((right_match[v] == -1 && distance[u] + 1 == shortest) ||
				(right_match[v] != -1 && distance[right_match[v]] == distance[u] + 1 &&
				 self(self, right_match[v]))) {
				left_match[u] = v;
				right_match[v] = u;
				return true;
			}
		}
		distance[u] = -1;
		return false;
	};
	int size = 0;
	while (true) {
		std::fill(current.begin(), current.end(), 0);
		shortest = left_size + 1;
		std::queue<int> queue;
		for (int u = 0; u < left_size; ++u) {
			distance[u] = left_match[u] == -1 ? 0 : -1;
			if (distance[u] == 0) queue.push(u);
		}
		while (!queue.empty()) {
			const int u = queue.front(); queue.pop();
			if (distance[u] >= shortest) continue;
			for (const int v : adj[u]) {
				assert(0 <= v && v < right_size);
				if (right_match[v] == -1) shortest = distance[u] + 1;
				else if (distance[right_match[v]] == -1) {
					distance[right_match[v]] = distance[u] + 1;
					queue.push(right_match[v]);
				}
			}
		}
		int augmented = 0;
		for (int u = 0; u < left_size; ++u)
			if (left_match[u] == -1 && dfs(dfs, u)) ++augmented;
		if (!augmented) break;
		size += augmented;
	}
	return {size, std::move(left_match), std::move(right_match)};
}

#endif
