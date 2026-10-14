#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/dsu.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	DisjointSetUnion dsu(n);
	while (q--) {
		int t, u, v;
		io >> t >> u >> v;
		if (t == 0) dsu.merge(u, v);
		else std::cout << dsu.same(u, v) << '\n';
	}
}
