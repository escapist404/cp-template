#define PROBLEM "https://judge.yosupo.jp/problem/tree_diameter"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "../../include/tree_diameter.hpp"

int main() {
	int n;
	std::cin >> n;
	std::vector<std::vector<std::pair<int, long long>>> graph(n);
	for (int i = 0; i + 1 < n; ++i) {
		int u, v;
		long long weight;
		std::cin >> u >> v >> weight;
		graph[u].push_back({v, weight});
		graph[v].push_back({u, weight});
	}
	auto diameter = tree_diameter(graph);
	int start = diameter.second.first, finish = diameter.second.second;
	std::vector<int> parent(n, -1);
	std::vector<long long> distance(n);
	std::vector<int> stack{start};
	while (!stack.empty()) {
		int u = stack.back();
		stack.pop_back();
		for (auto [v, weight] : graph[u]) if (v != parent[u]) {
			parent[v] = u;
			distance[v] = distance[u] + weight;
			stack.push_back(v);
		}
	}
	std::vector<int> path;
	for (int v = finish; v != -1; v = parent[v]) path.push_back(v);
	std::reverse(path.begin(), path.end());
	std::cout << diameter.first << ' ' << path.size() << '\n';
	for (int v : path) std::cout << v << ' ';
	std::cout << '\n';
}
