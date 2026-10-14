#define PROBLEM "https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/segment_tree_beats.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::vector<long long> a(n);
	for (auto& x : a) io >> x;
	SegmentTreeBeats<long long> tree(a);
	while (q--) {
		int type, l, r;
		long long b;
		io >> type >> l >> r;
		if (type == 3) std::cout << tree.sum(l, r) << '\n';
		else {
			io >> b;
			if (type == 0) tree.chmin(l, r, b);
			else if (type == 1) tree.chmax(l, r, b);
			else tree.add(l, r, b);
		}
	}
}
