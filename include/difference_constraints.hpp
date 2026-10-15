#ifndef DIFFERENCE_CONSTRAINTS_HPP
#define DIFFERENCE_CONSTRAINTS_HPP

#include <cassert>
#include <queue>
#include <utility>
#include <vector>

// 【差分约束系统 (difference_constraints.hpp)】
// - 0-based 变量编号 [0, n)，使用队列优化 Bellman-Ford (SPFA) 求解，最坏复杂度 O(nm)
// - 约束方向约定: DifferenceConstraint{u, v, w} 表示不等式 x[v] - x[u] <= w（即 x[v] <= x[u] + w）
//   - 若需 x[v] - x[u] >= w，可转化为 x[u] - x[v] <= -w，添加 {v, u, -w}
//   - 若需 x[v] - x[u] == w，同时添加 {u, v, w} 与 {v, u, -w}
// - 返回 {true, x} 表示有解并给出一组可行解 x；若存在负权环（矛盾）则返回 {false, {}}

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
