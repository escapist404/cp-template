#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <iostream>

#include "../../include/fenwick.hpp"

int main() {
	int n, q;
	std::cin >> n >> q;
	Fenwick<long long> fenwick(n);
	for (int i = 0; i < n; ++i) {
		long long value;
		std::cin >> value;
		fenwick.add(i, value);
	}
	while (q--) {
		int t;
		std::cin >> t;
		if (t == 0) {
			int p;
			long long value;
			std::cin >> p >> value;
			fenwick.add(p, value);
		} else {
			int l, r;
			std::cin >> l >> r;
			std::cout << fenwick.get(l, r) << '\n';
		}
	}
}
