#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/sparse_table.hpp"

struct Min {
	int operator()(int a, int b) const { return a < b ? a : b; }
};

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<int> a(n);
	for (int& x : a) io >> x;
	SparseTable<int, Min> table(a);
	while (q--) {
		int l, r;
		io >> l >> r;
		std::cout << table.query(l, r) << '\n';
	}
}
