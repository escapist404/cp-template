#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/binary_lifting_lca.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	BinaryLiftingLCA lca(n);
	for (int v = 1; v < n; ++v) {
		int parent;
		io >> parent;
		lca.add_edge(parent, v);
	}
	lca.prework(0);
	while (q--) {
		int u, v;
		io >> u >> v;
		std::cout << lca.lca(u, v) << '\n';
	}
}
