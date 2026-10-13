#define PROBLEM "https://judge.yosupo.jp/problem/scc"

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/scc.hpp"

int main() {
	int n, m;
	std::cin >> n >> m;
	std::vector<std::vector<int>> graph(n);
	while (m--) {
		int u, v;
		std::cin >> u >> v;
		graph[u].push_back(v);
	}
	auto components = get_scc(graph);
	std::reverse(components.begin(), components.end());
	std::cout << components.size() << '\n';
	for (const auto& component : components) {
		std::cout << component.size();
		for (int v : component) std::cout << ' ' << v;
		std::cout << '\n';
	}
}
