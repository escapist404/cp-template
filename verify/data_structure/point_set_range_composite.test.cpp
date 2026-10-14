#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include "../../include/fastio.hpp"
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
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<Affine> initial(n);
	for (auto& f : initial) io >> f.a >> f.b;
	SegmentTree<Affine, compose, identity> tree(initial);
	while (q--) {
		int type;
		io >> type;
		if (type == 0) {
			int p;
			Affine f;
			io >> p >> f.a >> f.b;
			tree.set(p, f);
		} else {
			int l, r;
			long long x;
			io >> l >> r >> x;
			Affine f = tree.prod(l, r);
			std::cout << (f.a * x + f.b) % MOD << '\n';
		}
	}
}
