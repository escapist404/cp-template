#define PROBLEM "https://judge.yosupo.jp/problem/eulerian_trail_undirected"

#include "../../include/fastio.hpp"
#include <iostream>
#include <utility>
#include <vector>

#include "../../include/euler_path.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int cases;
	io >> cases;
	while (cases--) {
		int n, m;
		io >> n >> m;
		std::vector<std::pair<int, int>> edges(m);
		for (auto& edge : edges) io >> edge.first >> edge.second;
		auto result = euler_path(n, edges);
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
