#ifndef DIFFERENCE_CONSTRAINTS_HPP
#define DIFFERENCE_CONSTRAINTS_HPP

#include <cassert>
#include <queue>
#include <utility>
#include <vector>

// ============================================================================
// 【差分约束系统求解器 Difference Constraints (difference_constraints.hpp)】
// ----------------------------------------------------------------------------
// 约束结构体签名：
//   template <typename T = long long> struct DifferenceConstraint { int u, v; T weight; };
//   【不等式方向约定】每条约束 {u, v, weight} 严格表示：
//       x[v] - x[u] <= weight   （即 x[v] <= x[u] + weight，对应有向边 u -> v 权 weight）
//   - 若需表示 x[v] - x[u] >= w，请转化为 x[u] - x[v] <= -w，添加 {v, u, -w}；
//   - 若需表示 x[v] - x[u] == w，请同时添加 {u, v, w} 与 {v, u, -w}。
// 求解函数签名：
//   template <typename T = long long>
//   std::pair<bool, std::vector<T>> difference_constraints(
//       int n, const std::vector<DifferenceConstraint<T>>& edges)
// 参数与返回值详解：
//   - n：变量个数，变量编号为 0-based [0, n)。
//   - edges：不等式约束列表。
//   - 返回值 std::pair{feasible, values}：
//     - 若系统无负权环（有解），返回 {true, x}，其中 x 为满足所有约束的一组可行解（初始全 0 超源点松弛出的最大非正解）；
//     - 若存在负权环（约束矛盾无解），返回 {false, {}}。
// 复杂度：基于 SPFA 判负环，最坏时间复杂度 O(n * m)，空间复杂度 O(n + m)。
// ============================================================================

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
