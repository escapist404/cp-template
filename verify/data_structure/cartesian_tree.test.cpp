#define PROBLEM "https://judge.yosupo.jp/problem/cartesian_tree"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/cartesian_tree.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	std::vector<int> a(n);
	for (int& x : a) io >> x;
	auto tree = cartesian_tree(a);
	for (int i = 0; i < n; ++i)
		std::cout << (i ? " " : "") << (tree.father[i] == -1 ? i : tree.father[i]);
	std::cout << '\n';
}
