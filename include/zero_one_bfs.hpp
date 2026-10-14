#ifndef ZERO_ONE_BFS_HPP
#define ZERO_ONE_BFS_HPP

#include <cassert>
#include <climits>
#include <deque>
#include <utility>
#include <vector>

// 0-based single-source shortest paths for graphs with edge weights in {0, 1} in O(n + m).

std::vector<int> zero_one_bfs(
		const std::vector<std::vector<std::pair<int, int>>>& adj, int source) {
	assert(0 <= source && source < (int)adj.size());
	std::vector<int> distance(adj.size(), INT_MAX);
	std::deque<int> queue;
	distance[source] = 0;
	queue.push_front(source);

	while (!queue.empty()) {
		int from = queue.front();
		queue.pop_front();
		for (const auto& [to, weight] : adj[from]) {
			assert(weight == 0 || weight == 1);
			if (distance[to] <= distance[from] + weight)
				continue;
			distance[to] = distance[from] + weight;
			if (weight == 0)
				queue.push_front(to);
			else
				queue.push_back(to);
		}
	}
	return distance;
}

#endif
