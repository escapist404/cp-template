#define PROBLEM "https://judge.yosupo.jp/problem/deque_operate_all_composite"

#include <iostream>

#include "../../include/fastio.hpp"
#include "../../include/modint.hpp"
#include "../../include/segtree.hpp"

using Z = ModInt<long long, 998244353>;

struct Affine {
	Z a, b;
};

Affine operation(Affine left, Affine right) {
	return {right.a * left.a, right.a * left.b + right.b};
}

Affine identity() { return {Z(1), Z(0)}; }

int main() {
	OptimizedIO<1 << 20> io;
	int q;
	io >> q;
	SegmentTree<Affine, operation, identity> tree(2 * q + 1);
	int left = q, right = q;
	while (q--) {
		int type;
		io >> type;
		if (type == 0 || type == 1) {
			long long a, b;
			io >> a >> b;
			if (type == 0) tree.set(--left, {Z(a), Z(b)});
			else tree.set(right++, {Z(a), Z(b)});
		} else if (type == 2) {
			++left;
		} else if (type == 3) {
			--right;
		} else {
			long long x;
			io >> x;
			Affine f = tree.prod(left, right);
			std::cout << f.a * Z(x) + f.b << '\n';
		}
	}
}
