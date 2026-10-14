#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_path_sum"

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
	for (int i = 0, u, v; i + 1 < n; ++i) {
		io >> u >> v;
		hld.add_edge(u, v);
	}
	hld.prework(0);
	Fenwick<long long> tree(n);
	for (int v = 0; v < n; ++v) tree.add(hld.pos(v), value[v]);
	auto path_sum = [&](int u, int v) {
		long long result = 0;
		while (hld.head(u) != hld.head(v)) {
			if (hld.depth_of(hld.head(u)) < hld.depth_of(hld.head(v))) std::swap(u, v);
			result += tree.get(hld.pos(hld.head(u)), hld.pos(u) + 1);
			u = hld.parent(hld.head(u));
		}
		int l = std::min(hld.pos(u), hld.pos(v));
		int r = std::max(hld.pos(u), hld.pos(v)) + 1;
		return result + tree.get(l, r);
	};
	while (q--) {
		int type, u, v;
		io >> type >> u >> v;
		if (type == 0) tree.add(hld.pos(u), v);
		else std::cout << path_sum(u, v) << '\n';
	}
}
