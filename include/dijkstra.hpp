#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <cassert>
#include <functional>
#include <initializer_list>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

// ============================================================================
// 【Dijkstra 单源/多源非负权最短路 (dijkstra.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename T>
//   std::vector<T> dijkstra(const std::vector<std::vector<std::pair<int, T>>>& adj,
//                           std::initializer_list<int> sources)
// 参数与返回值详解：
//   - adj：0-based 有向或无向带权图邻接表，adj[u] 存储从 u 出发的边 {终点 v, 边权 weight}。
//     【前置条件】所有边权必须满足 weight >= 0（内部含 assert(weight >= T())）。
//   - sources：起点初始化列表，单源最短路传 {s}，多源最短路传 {s1, s2, ...}。
//   - 返回值 std::vector<T>：长度为 n = adj.size() 的最短距离数组 distance：
//     - 各起点处 distance[s] = 0；
//     - 不可达顶点处 distance[v] == std::numeric_limits<T>::max()（内部自带加法溢出保护，无需改小无穷大常数）。
// 复杂度：时间复杂度 O((n + m) log m)，空间复杂度 O(n + m)。
// ============================================================================

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
