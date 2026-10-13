#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"

#include <iostream>

#include "../../include/dsu.hpp"

int main() {
	int n, q;
	std::cin >> n >> q;
	DisjointSetUnion dsu(n);
	while (q--) {
		int t, u, v;
		std::cin >> t >> u >> v;
		if (t == 0) dsu.merge(u, v);
		else std::cout << dsu.same(u, v) << '\n';
	}
}
