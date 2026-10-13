#define PROBLEM "https://judge.yosupo.jp/problem/biconnected_components"

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/vdcc.hpp"

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
	auto components = get_vdcc(graph);
	std::vector<int> isolated;
	for (int v = 0; v < n; ++v)
		if (graph[v].empty()) isolated.push_back(v);
	int isolated_index = 0;
	std::cout << components.size() << '\n';
	for (auto& component : components) {
		std::vector<int> vertices;
		for (auto [u, v] : component) {
			vertices.push_back(u);
			vertices.push_back(v);
		}
		std::sort(vertices.begin(), vertices.end());
		vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
		if (vertices.empty()) vertices.push_back(isolated[isolated_index++]);
		std::cout << vertices.size();
		for (int v : vertices) std::cout << ' ' << v;
		std::cout << '\n';
	}
}
