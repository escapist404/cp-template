#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/wavelet_tree.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<int> a(n);
	for (int& x : a) io >> x;
	WaveletTree<int> tree(a);
	while (q--) {
		int l, r, k;
		io >> l >> r >> k;
		std::cout << tree.kth(l, r, k) << '\n';
	}
}
