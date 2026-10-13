#define PROBLEM "https://judge.yosupo.jp/problem/unionfind_with_potential"

#include <iostream>

#include "../../include/modint.hpp"
#include "../../include/weighted_dsu.hpp"

int main() {
	using Z = ModInt<long long, 998244353>;
	int n, q;
	std::cin >> n >> q;
	WeightedDSU<Z> dsu(n);
	while (q--) {
		int type, u, v;
		std::cin >> type >> u >> v;
		if (type == 0) {
			long long x;
			std::cin >> x;
			std::cout << dsu.merge(v, u, Z(x)) << '\n';
		} else if (!dsu.same(u, v)) {
			std::cout << -1 << '\n';
		} else {
			std::cout << dsu.difference(v, u) << '\n';
		}
	}
}
