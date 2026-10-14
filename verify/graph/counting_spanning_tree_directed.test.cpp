#define PROBLEM "https://judge.yosupo.jp/problem/counting_spanning_tree_directed"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/determinant.hpp"
#include "../../include/matrix.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n, m, root;
	io >> n >> m >> root;
	if (n == 1) {
		while (m--) { int u, v; io >> u >> v; }
		std::cout << 1 << '\n';
		return 0;
	}
	Matrix<Z, 0, 0> laplacian(n, n);
	for (int i = 0, u, v; i < m; ++i) {
		io >> u >> v;
		laplacian[v][v] += Z(1);
		laplacian[v][u] -= Z(1);
	}
	Matrix<Z, 0, 0> minor(n - 1, n - 1);
	for (int i = 0, ii = 0; i < n; ++i) if (i != root) {
		for (int j = 0, jj = 0; j < n; ++j) if (j != root) {
			minor[ii][jj++] = laplacian[i][j];
		}
		++ii;
	}
	std::cout << determinant(minor) << '\n';
}
