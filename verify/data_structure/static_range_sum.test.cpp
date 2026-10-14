#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/fenwick.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<long long> a(n);
	for (auto& x : a) io >> x;
	Fenwick<long long> tree(a);
	while (q--) {
		int l, r;
		io >> l >> r;
		std::cout << tree.get(l, r) << '\n';
	}
}
