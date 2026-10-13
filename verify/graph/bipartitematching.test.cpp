#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"

#include <iostream>
#include <vector>

#include "../../include/bipartite_matching.hpp"

int main() {
	int left_size, right_size, m;
	std::cin >> left_size >> right_size >> m;
	std::vector<std::vector<int>> graph(left_size);
	while (m--) {
		int u, v;
		std::cin >> u >> v;
		graph[u].push_back(v);
	}
	auto result = bipartite_matching(graph, right_size);
	std::cout << result.size << '\n';
	for (int u = 0; u < left_size; ++u)
		if (result.left[u] != -1) std::cout << u << ' ' << result.left[u] << '\n';
}
