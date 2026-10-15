#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <cassert>
#include <functional>
#include <initializer_list>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

// 【Dijkstra 最短路 (dijkstra.hpp)】
// - 0-based 邻接表 adj[u] = {{v, weight}, ...}，要求所有边权非负 (weight >= 0)，时间复杂度 O((n + m) log m)
// - 支持单源或多源同时出发：sources 传入初始化列表（如 {s} 或 {s1, s2}）
// - 返回长度为 n 的距离数组：起点距离为 0，不可达顶点距离为 std::numeric_limits<T>::max()（内部自带防加权溢出保护）

template <typename T>
std::vector<T> dijkstra(
		const std::vector<std::vector<std::pair<int, T>>>& adj,
		const std::initializer_list<int> sources) {

	const T inf = std::numeric_limits<T>::max();
	std::vector<T> distance(adj.size(), inf);

	using State = std::pair<T, int>;
	std::priority_queue<
		State,
		std::vector<State>,
		std::greater<State>> queue;

	for (const int source : sources) {
		assert(0 <= source && source < (int)(adj.size()));
		distance[source] = T();
		queue.emplace(T(), source);
	}

	while (!queue.empty()) {
		const auto [current_distance, from] = queue.top();
		queue.pop();

		if (current_distance != distance[from]) {
			continue;
		}

		for (const auto& [to, weight] : adj[from]) {
			// Dijkstra 要求非负边权；保留 inf 为类型最大值，
			// 仅跳过会导致有符号加权溢出的转移
			if (weight > T() && current_distance > inf - weight) {
				continue;
			}
			const T candidate = current_distance + weight;
			if (candidate >= distance[to]) {
				continue;
			}

			distance[to] = candidate;
			queue.emplace(candidate, to);
		}
	}

	return distance;
}

#endif
