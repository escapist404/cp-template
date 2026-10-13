#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <iostream>
#include <vector>

#include "../../include/sparse_table.hpp"

struct Min {
	int operator()(int a, int b) const { return a < b ? a : b; }
};

int main() {
	int n, q;
	std::cin >> n >> q;
	std::vector<int> a(n);
	for (int& x : a) std::cin >> x;
	SparseTable<int, Min> table(a);
	while (q--) {
		int l, r;
		std::cin >> l >> r;
		std::cout << table.query(l, r) << '\n';
	}
}
