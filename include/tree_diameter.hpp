#ifndef TREE_DIAMETER_HPP
#define TREE_DIAMETER_HPP

#include <utility>
#include <vector>

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
