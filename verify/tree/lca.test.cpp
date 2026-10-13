#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include <iostream>

#include "../../include/binary_lifting_lca.hpp"

int main() {
	int n, q;
	std::cin >> n >> q;
	BinaryLiftingLCA lca(n);
	for (int v = 1; v < n; ++v) {
		int parent;
		std::cin >> parent;
		lca.add_edge(parent, v);
	}
	lca.prework(0);
	while (q--) {
		int u, v;
		std::cin >> u >> v;
		std::cout << lca.lca(u, v) << '\n';
	}
}
