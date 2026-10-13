#define PROBLEM "https://judge.yosupo.jp/problem/eulerian_trail_directed"

#include <iostream>
#include <utility>
#include <vector>

#include "../../include/euler_path.hpp"

int main() {
	int cases;
	std::cin >> cases;
	while (cases--) {
		int n, m;
		std::cin >> n >> m;
		std::vector<std::pair<int, int>> edges(m);
		for (auto& edge : edges) std::cin >> edge.first >> edge.second;
		auto result = euler_path(n, edges, true);
		if (!result.exists) {
			std::cout << "No\n";
			continue;
		}
		std::cout << "Yes\n";
		for (int v : result.vertices) std::cout << v << ' ';
		std::cout << '\n';
		for (int edge : result.edges) std::cout << edge << ' ';
		std::cout << '\n';
	}
}
