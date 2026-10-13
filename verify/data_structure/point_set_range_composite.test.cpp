#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include <iostream>
#include <vector>

#include "../../include/segtree.hpp"

const long long MOD = 998244353;

struct Affine {
	long long a, b;
};

Affine compose(Affine left, Affine right) {
	return {left.a * right.a % MOD,
			(left.b * right.a + right.b) % MOD};
}

Affine identity() { return {1, 0}; }

int main() {
	int n, q;
	std::cin >> n >> q;
	std::vector<Affine> initial(n);
	for (auto& f : initial) std::cin >> f.a >> f.b;
	SegmentTree<Affine, compose, identity> tree(initial);
	while (q--) {
		int type;
		std::cin >> type;
		if (type == 0) {
			int p;
			Affine f;
			std::cin >> p >> f.a >> f.b;
			tree.set(p, f);
		} else {
			int l, r;
			long long x;
			std::cin >> l >> r >> x;
			Affine f = tree.prod(l, r);
			std::cout << (f.a * x + f.b) % MOD << '\n';
		}
	}
}
