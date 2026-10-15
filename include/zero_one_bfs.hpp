#ifndef ZERO_ONE_BFS_HPP
#define ZERO_ONE_BFS_HPP

#include <cassert>
#include <climits>
#include <deque>
#include <utility>
#include <vector>

// 【0-1 BFS 最短路 (zero_one_bfs.hpp)】
// - 0-based 邻接表 adj[u] = {{v, weight}, ...}，要求所有边权严格为 0 或 1，时间复杂度严格线性 O(n + m)
// - 返回从 source 出发的最短距离数组，不可达顶点距离为 INT_MAX

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
