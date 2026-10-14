#define PROBLEM "https://judge.yosupo.jp/problem/range_reverse_range_sum"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/implicit_treap.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<long long> a(n);
	for (auto& x : a) io >> x;
	ImplicitTreap<long long> tree(a);
	while (q--) {
		int type, l, r;
		io >> type >> l >> r;
		if (type == 0) tree.reverse(l, r);
		else std::cout << tree.query(l, r) << '\n';
	}
}
