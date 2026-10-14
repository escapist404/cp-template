#define PROBLEM "https://judge.yosupo.jp/problem/matrix_rank"

#include "../../include/fastio.hpp"
#include <iostream>
#include <vector>

#include "../../include/gaussian.hpp"
#include "../../include/modint.hpp"

using Z = ModInt<long long, 998244353>;

int main() {
	OptimizedIO<1 << 20> io;
	int n, m;
	io >> n >> m;
	if (n == 0 || m == 0) {
		std::cout << 0 << '\n';
		return 0;
	}
	Matrix<Z, 0, 0> matrix(n, m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) io >> matrix[i][j];
	std::cout << matrix.rank() << '\n';
}
