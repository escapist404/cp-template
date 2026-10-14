#define PROBLEM "https://judge.yosupo.jp/problem/longest_increasing_subsequence"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "../../include/coordinate_compression.hpp"
#include "../../include/fastio.hpp"
#include "../../include/segtree.hpp"

using Node = std::pair<int, int>;
Node operation(Node a, Node b) { return a.first >= b.first ? a : b; }
Node identity() { return {0, -1}; }

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	std::vector<int> a(n);
	for (auto& x : a) io >> x;
	auto [values, compressed] = coordinate_compress(a);
	a = std::move(compressed);
	SegmentTree<Node, operation, identity> tree(values.size());
	std::vector<int> parent(n, -1);
	for (int i = 0; i < n; ++i) {
		Node previous = tree.prod(0, a[i]);
		parent[i] = previous.second;
		Node current{previous.first + 1, i};
		if (tree.get(a[i]).first < current.first) tree.set(a[i], current);
	}
	Node best = tree.all_prod();
	std::vector<int> answer;
	for (int i = best.second; i != -1; i = parent[i]) answer.push_back(i);
	std::reverse(answer.begin(), answer.end());
	std::cout << answer.size() << '\n';
	for (int i : answer) std::cout << i << ' ';
	std::cout << '\n';
}
