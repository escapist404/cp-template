#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"

#include <iostream>
#include <vector>

#include "../../include/wavelet_tree.hpp"

int main() {
	int n, q;
	std::cin >> n >> q;
	std::vector<int> a(n);
	for (int& x : a) std::cin >> x;
	WaveletTree<int> tree(a);
	while (q--) {
		int l, r, k;
		std::cin >> l >> r >> k;
		std::cout << tree.kth(l, r, k) << '\n';
	}
}
