#define PROBLEM "https://judge.yosupo.jp/problem/counting_spanning_tree_undirected"

#include "../../include/fastio.hpp"
#include <iostream>
#include <utility>
#include <vector>

#include "../../include/matrix_tree.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n, m;
	io >> n >> m;
	std::vector<std::pair<int, int>> edges(m);
	for (auto& [u, v] : edges) io >> u >> v;
	std::cout << matrix_tree<Z>(n, edges) << '\n';
}
