#define PROBLEM "https://judge.yosupo.jp/problem/queue_operate_all_composite"

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
	SegmentTree<Affine, operation, identity> tree(q);
	int head = 0, tail = 0;
	while (q--) {
		int type;
		io >> type;
		if (type == 0) {
			long long a, b;
			io >> a >> b;
			tree.set(tail++, {Z(a), Z(b)});
		} else if (type == 1) {
			++head;
		} else {
			long long x;
			io >> x;
			Affine f = tree.prod(head, tail);
			std::cout << f.a * Z(x) + f.b << '\n';
		}
	}
}
