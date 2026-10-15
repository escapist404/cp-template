#ifndef TREE_DIAMETER_HPP
#define TREE_DIAMETER_HPP

#include <utility>
#include <vector>

// 【树的直径 (tree_diameter.hpp)】
// - 0-based 连通无向树，通过两次 DFS 在 O(n) 时间内求直径（要求所有树边权非负）
// - tree_diameter_farthest(adj, start): 返回 {最远距离, 最远端点}
// - tree_diameter(adj)                : 支持带权邻接表或无权邻接表（默认边权 1），返回 {直径长度, {端点 u, 端点 v}}

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
