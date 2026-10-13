#define PROBLEM "https://judge.yosupo.jp/problem/two_edge_connected_components"

#include <iostream>
#include <vector>

#include "../../include/edcc.hpp"

int main() {
	int n, m;
	std::cin >> n >> m;
	std::vector<std::vector<int>> graph(n);
	while (m--) {
		int u, v;
		std::cin >> u >> v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	auto components = get_edcc(graph);
	std::cout << components.size() << '\n';
	for (const auto& component : components) {
		std::cout << component.size();
		for (int v : component) std::cout << ' ' << v;
		std::cout << '\n';
	}
}
