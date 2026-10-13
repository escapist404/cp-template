#define PROBLEM "https://judge.yosupo.jp/problem/minimum_spanning_tree"

#include <iostream>
#include <map>
#include <tuple>
#include <vector>

#include "../../include/kruskal.hpp"

int main() {
	int n, m;
	std::cin >> n >> m;
	using Key = std::tuple<int, int, long long>;
	std::map<Key, std::vector<int>> ids;
	std::vector<KruskalEdge<long long>> edges;
	for (int i = 0; i < m; ++i) {
		int u, v;
		long long weight;
		std::cin >> u >> v >> weight;
		edges.push_back({u, v, weight});
		ids[{u, v, weight}].push_back(i);
	}
	auto result = kruskal(n, edges);
	std::cout << result.first << '\n';
	for (const auto& edge : result.second) {
		auto& choices = ids[{edge.u, edge.v, edge.weight}];
		int id = choices.back();
		choices.pop_back();
		std::cout << id << ' ';
	}
	std::cout << '\n';
}
