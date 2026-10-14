#define PROBLEM "https://judge.yosupo.jp/problem/range_add_range_min"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/lazy_segtree.hpp"

using Value = long long;
using Lazy = long long;

Value operation(Value a, Value b) { return a < b ? a : b; }
Value identity_value() { return (Value)4e18; }
Value mapping(Lazy f, Value x) { return x + f; }
Lazy composition(Lazy f, Lazy g) { return f + g; }
Lazy identity_lazy() { return 0; }

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<Value> a(n);
	for (auto& x : a) io >> x;
	LazySegmentTree<Value, operation, identity_value, Lazy,
			mapping, composition, identity_lazy> tree(a);
	while (q--) {
		int type, l, r;
		io >> type >> l >> r;
		if (type == 0) {
			Lazy x;
			io >> x;
			tree.apply(l, r, x);
		} else {
			std::cout << tree.prod(l, r) << '\n';
		}
	}
}
