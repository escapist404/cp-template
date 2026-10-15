#ifndef TREE_DIAMETER_HPP
#define TREE_DIAMETER_HPP

#include <utility>
#include <vector>

// ============================================================================
// 【树的直径（两次 DFS 法）Tree Diameter (tree_diameter.hpp)】
// ----------------------------------------------------------------------------
// 函数签名详解（0-based 顶点编号 [0, n)，要求输入为连通无向树且所有边权非负）：
//   1. template <typename T>
//      std::pair<T, int> tree_diameter_farthest(
//          const std::vector<std::vector<std::pair<int, T>>>& adj, int start)
//      功能：从顶点 start 出发做一次 DFS，返回 {最大距离, 距离 start 最远的顶点编号}。时间复杂度 O(n)。
//   2. template <typename T>
//      std::pair<T, std::pair<int, int>> tree_diameter(
//          const std::vector<std::vector<std::pair<int, T>>>& adj)
//      功能：计算带权树 adj（adj[u] = {{v, w}, ...}）的直径。
//      返回：{直径总长度, {直径端点 u, 直径端点 v}}（空树返回 {0, {-1, -1}}）。时间复杂度 O(n)。
//   3. inline std::pair<int, std::pair<int, int>> tree_diameter(
//          const std::vector<std::vector<int>>& adj)
//      功能：无权树重载（每条树边权视为 1），返回 {直径边数, {端点 u, 端点 v}}。时间复杂度 O(n)。
// ============================================================================

// template <typename T> std::pair<T, int> tree_diameter_farthest( const std::vector<std::vector<std::pair<int, T>>>& adj, int start) — 功能：从顶点 start 出发做一次 DFS，返回 {最大距离, 距离 start 最远的顶点编号}。时间复杂度 O(n)。
template <typename T>
std::pair<T, int> tree_diameter_farthest(const std::vector<std::vector<std::pair<int, T>>>& adj,
			int start) {
		std::pair<T, int> result{T(), start};
		std::vector<std::pair<int, int>> stack{{start, -1}};
		std::vector<T> distance(adj.size());
		while (!stack.empty()) {
			auto [from, parent] = stack.back();
			stack.pop_back();
			if (distance[from] > result.first)
				result = {distance[from], from};
			for (const auto& [to, weight] : adj[from]) {
				if (to == parent)
					continue;
				distance[to] = distance[from] + weight;
				stack.push_back({to, from});
			}
		}
		return result;
	}

template <typename T>
std::pair<T, std::pair<int, int>> tree_diameter(
		const std::vector<std::vector<std::pair<int, T>>>& adj) {
	if (adj.empty())
		return {T(), {-1, -1}};
	auto first = tree_diameter_farthest(adj, 0);
	auto second = tree_diameter_farthest(adj, first.second);
	return {second.first, {first.second, second.second}};
}

inline std::pair<int, std::pair<int, int>> tree_diameter(
		const std::vector<std::vector<int>>& adj) {
	std::vector<std::vector<std::pair<int, int>>> weighted(adj.size());
	for (int u = 0; u < (int)adj.size(); ++u)
		for (int v : adj[u])
			weighted[u].push_back({v, 1});
	return tree_diameter(weighted);
}

#endif
