#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_point_get"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/lazy_segtree.hpp"

const long long MOD = 998244353;
using Value = long long;
using Lazy = std::pair<long long, long long>;

Value operation(Value a, Value b) { return (a + b) % MOD; }
Value identity_value() { return 0; }
Value mapping(Lazy f, Value x) { return (f.first * x + f.second) % MOD; }
Lazy composition(Lazy f, Lazy g) {
	return {f.first * g.first % MOD,
			(f.first * g.second + f.second) % MOD};
}
Lazy identity_lazy() { return {1, 0}; }

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<Value> a(n);
	for (auto& x : a) io >> x;
	LazySegmentTree<Value, operation, identity_value, Lazy,
			mapping, composition, identity_lazy> tree(a);
	while (q--) {
		int type;
		io >> type;
		if (type == 0) {
			int l, r;
			Lazy f;
			io >> l >> r >> f.first >> f.second;
			tree.apply(l, r, f);
		} else {
			int i;
			io >> i;
			std::cout << tree.get(i) << '\n';
		}
	}
}
