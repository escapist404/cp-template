#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_subtree_sum"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/fenwick.hpp"
#include "../../include/hld.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<long long> value(n);
	for (auto& x : value) io >> x;
	HeavyLightDecomposition hld(n);
	for (int v = 1, p; v < n; ++v) {
		io >> p;
		hld.add_edge(v, p);
	}
	hld.prework(0);
	Fenwick<long long> tree(n);
	for (int v = 0; v < n; ++v) tree.add(hld.pos(v), value[v]);
	while (q--) {
		int type, u;
		io >> type >> u;
		if (type == 0) {
			long long x;
			io >> x;
			tree.add(hld.pos(u), x);
		} else {
			auto [l, r] = hld.subtree_range(u);
			std::cout << tree.get(l, r) << '\n';
		}
	}
}
