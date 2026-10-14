#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/link_cut_tree.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	LinkCutTree<long long> tree(n);
	for (int v = 1; v <= n; ++v) {
		long long value;
		io >> value;
		tree.set_value(v, value);
	}
	for (int i = 0, u, v; i + 1 < n; ++i) {
		io >> u >> v;
		tree.link(u + 1, v + 1);
	}
	while (q--) {
		int type;
		io >> type;
		if (type == 0) {
			int u, v, w, x;
			io >> u >> v >> w >> x;
			tree.cut(u + 1, v + 1);
			tree.link(w + 1, x + 1);
		} else if (type == 1) {
			int p;
			long long x;
			io >> p >> x;
			tree.set_value(p + 1, tree.value(p + 1) + x);
		} else {
			int u, v;
			io >> u >> v;
			std::cout << tree.path_query(u + 1, v + 1) << '\n';
		}
	}
}
