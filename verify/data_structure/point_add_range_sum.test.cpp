#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include "../../include/fastio.hpp"
#include <iostream>

#include "../../include/fenwick.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	Fenwick<long long> fenwick(n);
	for (int i = 0; i < n; ++i) {
		long long value;
		io >> value;
		fenwick.add(i, value);
	}
	while (q--) {
		int t;
		io >> t;
		if (t == 0) {
			int p;
			long long value;
			io >> p >> value;
			fenwick.add(p, value);
		} else {
			int l, r;
			io >> l >> r;
			std::cout << fenwick.get(l, r) << '\n';
		}
	}
}
