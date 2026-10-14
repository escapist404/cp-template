#define PROBLEM "https://judge.yosupo.jp/problem/range_set_range_composite"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/lazy_segtree.hpp"

const long long MOD = 998244353;

struct Affine {
	long long a, b;
};
struct Value {
	Affine f;
	int length;
};
struct Lazy {
	Affine f;
	bool set;
};

Affine compose(Affine left, Affine right) {
	return {left.a * right.a % MOD,
			(left.b * right.a + right.b) % MOD};
}
Affine power(Affine f, int n) {
	Affine result{1, 0};
	while (n) {
		if (n & 1) result = compose(result, f);
		f = compose(f, f);
		n >>= 1;
	}
	return result;
}
Value operation(Value left, Value right) {
	return {compose(left.f, right.f), left.length + right.length};
}
Value identity_value() { return {{1, 0}, 0}; }
Value mapping(Lazy lazy, Value value) {
	return {lazy.set ? power(lazy.f, value.length) : value.f, value.length};
}
Lazy composition(Lazy newer, Lazy older) {
	return newer.set ? newer : older;
}
Lazy identity_lazy() { return {{1, 0}, false}; }

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<Value> initial(n);
	for (auto& value : initial) {
		io >> value.f.a >> value.f.b;
		value.length = 1;
	}
	LazySegmentTree<Value, operation, identity_value, Lazy,
			mapping, composition, identity_lazy> tree(initial);
	while (q--) {
		int type, l, r;
		io >> type >> l >> r;
		if (type == 0) {
			Lazy lazy;
			io >> lazy.f.a >> lazy.f.b;
			lazy.set = true;
			tree.apply(l, r, lazy);
		} else {
			long long x;
			io >> x;
			Affine f = tree.prod(l, r).f;
			std::cout << (f.a * x + f.b) % MOD << '\n';
		}
	}
}
