#ifndef ZERO_ONE_BFS_HPP
#define ZERO_ONE_BFS_HPP

#include <cassert>
#include <climits>
#include <deque>
#include <utility>
#include <vector>

// ============================================================================
// 【0-1 BFS 双端队列最短路 (zero_one_bfs.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   std::vector<int> zero_one_bfs(const std::vector<std::vector<std::pair<int, int>>>& adj, int source)
// 参数与返回值详解：
//   - adj：0-based 邻接表，adj[u] 存储 {to, weight}；【前置条件】所有边权 weight 必须严格为 0 或 1（否则触发 assert）。
//   - source：源点编号（要求 0 <= source < adj.size()）。
//   - 返回值 std::vector<int>：长度为 n 的最短路数组，不可达顶点距离为 INT_MAX。
// 复杂度：每个顶点和边最多入队一次，时间和空间复杂度均为严格线性 O(n + m)。
// ============================================================================

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
