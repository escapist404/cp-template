#define PROBLEM "https://judge.yosupo.jp/problem/general_matching"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/blossom.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, m;
	io >> n >> m;
	GeneralMatching matching(n);
	while (m--) {
		int u, v;
		io >> u >> v;
		matching.add_edge(u, v);
	}
	int size = matching.max_matching();
	const auto& mate = matching.matching();
	std::cout << size << '\n';
	for (int u = 0; u < n; ++u)
		if (u < mate[u]) std::cout << u << ' ' << mate[u] << '\n';
}
