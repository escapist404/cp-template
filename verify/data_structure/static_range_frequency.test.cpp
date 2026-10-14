#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/wavelet_tree.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<int> a(n);
	for (auto& x : a) io >> x;
	WaveletTree<int> tree(a);
	while (q--) {
		int l, r, x;
		io >> l >> r >> x;
		std::cout << tree.count(l, r, x, x + 1) << '\n';
	}
}
